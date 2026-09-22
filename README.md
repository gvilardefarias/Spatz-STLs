# Spatz-STLs
Software Test Libraries (STLs) for the Spatz vector accelerator/`processor (VPU)

## Editions to make it work
clone in /sw folder

change /spatz/hw/system/spatz_cluster/sw/CMakeLists.txt
add_subdirectory(${SNITCH_SOFTWARE_DIR}/Spatz-STLs spatzSTLs)