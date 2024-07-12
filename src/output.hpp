#pragma once

#include "position.hpp"

#include "H5Cpp.h"

#include <format>

class Output {
public:
    Output(std::string name)
        : name_ { name }
        , h5file_ { std::format("{}.h5", name), H5F_ACC_TRUNC }
        , cell_entry_type_ { sizeof(Cell) }
        , update_entry_type_ { sizeof(Status) }
    {
        cell_entry_type_.insertMember("cell_index", HOFFSET(Cell, index), H5::PredType::NATIVE_INT);
        cell_entry_type_.insertMember("parent_index", HOFFSET(Cell, parent), H5::PredType::NATIVE_INT);
        cell_entry_type_.insertMember("cell_x", HOFFSET(Cell, pos_x), H5::PredType::NATIVE_FLOAT);
        cell_entry_type_.insertMember("cell_y", HOFFSET(Cell, pos_y), H5::PredType::NATIVE_FLOAT);
        cell_entry_type_.insertMember("cell_z", HOFFSET(Cell, pos_z), H5::PredType::NATIVE_FLOAT);

        update_entry_type_.insertMember("cell_index", HOFFSET(Status, index), H5::PredType::NATIVE_INT);
        update_entry_type_.insertMember("tic", HOFFSET(Status, tic), H5::PredType::NATIVE_INT);
        update_entry_type_.insertMember("repro", HOFFSET(Status, repro), H5::PredType::NATIVE_INT);
        update_entry_type_.insertMember("attempt", HOFFSET(Status, attempt), H5::PredType::NATIVE_INT);
        update_entry_type_.insertMember("is_stem", HOFFSET(Status, is_stem), H5::PredType::NATIVE_HBOOL);
        update_entry_type_.insertMember("is_active", HOFFSET(Status, is_active), H5::PredType::NATIVE_HBOOL);
    }

    void addCell(int index, int parent, Position pos)
    {
        cells_.emplace_back(index, parent, pos.x, pos.y, pos.z);
    }

    void updateStatus(int index, int tic, int repro, int attempt, bool is_stem, bool is_active)
    {
        updates_.emplace_back(index, tic, repro, attempt, is_stem, is_active);
    }

    virtual ~Output()
    {
        writeOutput();
    }

private:
    void writeOutput()
    {
        std::string cells_name = "cells";
        hsize_t cells_dim = cells_.size();
        H5::DataSpace cell_data_space(1, &cells_dim);
        H5::DataSet cell_data_set(h5file_.createDataSet(cells_name, cell_entry_type_, cell_data_space));
        cell_data_set.write(cells_.data(), cell_entry_type_);

        std::string updates_name = "updates";
        hsize_t updates_dim = updates_.size();
        H5::DataSpace update_data_space(1, &updates_dim);
        H5::DataSet update_data_set(h5file_.createDataSet(updates_name, update_entry_type_, update_data_space));
        update_data_set.write(updates_.data(), update_entry_type_);
    }

    std::string name_;
    H5::H5File h5file_;
    H5::CompType cell_entry_type_;
    H5::CompType update_entry_type_;

    struct Cell {
        int index;
        int parent;
        float pos_x;
        float pos_y;
        float pos_z;
    };

    struct Status {
        int index;
        int tic;
        int repro;
        int attempt;
        bool is_stem;
        bool is_active;
    };

    std::vector<Cell> cells_;
    std::vector<Status> updates_;
};
