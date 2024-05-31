#pragma once

#include "position.hpp"

#include "H5Cpp.h"

#include <format>

class Output {
public:
    Output(std::string name)
        : name_ { name }
        , h5file_ { std::format("{}.h5", name), H5F_ACC_TRUNC }
        , entry_type_ { sizeof(Entry) }
    {
        entry_type_.insertMember("tic", HOFFSET(Entry, tic), H5::PredType::NATIVE_INT);
        entry_type_.insertMember("repro", HOFFSET(Entry, repro), H5::PredType::NATIVE_INT);
        entry_type_.insertMember("attempt", HOFFSET(Entry, attempt), H5::PredType::NATIVE_INT);
        entry_type_.insertMember("cell_x", HOFFSET(Entry, pos_x), H5::PredType::NATIVE_FLOAT);
        entry_type_.insertMember("cell_y", HOFFSET(Entry, pos_y), H5::PredType::NATIVE_FLOAT);
        entry_type_.insertMember("cell_z", HOFFSET(Entry, pos_z), H5::PredType::NATIVE_FLOAT);
        entry_type_.insertMember("is_stem", HOFFSET(Entry, is_stem), H5::PredType::NATIVE_HBOOL);
        entry_type_.insertMember("is_active", HOFFSET(Entry, is_active), H5::PredType::NATIVE_HBOOL);
    }

    void add(int tic, int repro, int attempt, Position pos, bool is_stem, bool is_active)
    {
        output_.emplace_back(tic, repro, attempt, pos.x, pos.y, pos.z, is_stem, is_active);
    }

    virtual ~Output()
    {
        flushOutput();
    }

private:
    void flushOutput()
    {
        hsize_t dim = output_.size();
        H5::DataSpace data_space(1, &dim);
        H5::DataSet data_set(h5file_.createDataSet(name_, entry_type_, data_space));
        data_set.write(output_.data(), entry_type_);
    }

    std::string name_;
    H5::H5File h5file_;
    H5::CompType entry_type_;

    struct Entry {
        int tic;
        int repro;
        int attempt;
        float pos_x;
        float pos_y;
        float pos_z;
        bool is_stem;
        bool is_active;
    };

    std::vector<Entry> output_;
};
