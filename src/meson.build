pcg_proj = subproject('pcg')
pcg_dep = pcg_proj.get_variable('pcg_cpp_dep')

cxxopts_proj = subproject('cxxopts')
cxxopts_dep = cxxopts_proj.get_variable('cxxopts_dep')

hdf5_dep = dependency('hdf5', language: 'cpp')

omp_dep = dependency('openmp')

sources = [ 'cli.cpp', 'culture.cpp', 'grid.cpp', 'neighborhood.cpp' ]

tumor2 = executable('tumor2',
  sources,
  dependencies: [ cxxopts_dep, pcg_dep, hdf5_dep, omp_dep ]
)