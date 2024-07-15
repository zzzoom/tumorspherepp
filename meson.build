project('tumorsphere++', 'cpp',
        default_options: [ 'cpp_std=c++23',
                           'buildtype=release',
                           'warning_level=2'
                         ],
        meson_version: '>= 1.0'
)

pcg_proj = subproject('pcg')
pcg_dep = pcg_proj.get_variable('pcg_cpp_dep')

cxxopts_proj = subproject('cxxopts')
cxxopts_dep = cxxopts_proj.get_variable('cxxopts_dep')

hdf5_dep = dependency('hdf5', language: 'cpp')

omp_dep = dependency('openmp')

sources = [
  'src/cli.cpp',
  'src/culture.cpp',
  'src/grid.cpp',
  'src/neighborhood.cpp'
]

tumorspherepp = executable('tumorsphere++',
  sources,
  dependencies: [ cxxopts_dep, pcg_dep, hdf5_dep, omp_dep ],
  install: true
)
