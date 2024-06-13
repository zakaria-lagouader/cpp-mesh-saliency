if(TARGET xtensor)
    return()
endif()

include(FetchContent)

# Fetch xtl dependency
FetchContent_Declare(
    xtl
    GIT_REPOSITORY https://github.com/xtensor-stack/xtl.git
    GIT_TAG 0.7.5  # Specify the required version
)
FetchContent_MakeAvailable(xtl)

# Fetch xtensor library
FetchContent_Declare(
    xtensor
    GIT_REPOSITORY https://github.com/xtensor-stack/xtensor.git
    GIT_TAG 0.24.5  # Specify the required version
)
FetchContent_MakeAvailable(xtensor)

# Fetch xtensor-io library
FetchContent_Declare(
    xtensor_io
    GIT_REPOSITORY https://github.com/xtensor-stack/xtensor-io.git
    GIT_TAG 0.13.0  # Specify the required version
)
FetchContent_MakeAvailable(xtensor_io)

# Fetch xtensor-blas library
FetchContent_Declare(
    xtensor_blas
    GIT_REPOSITORY https://github.com/xtensor-stack/xtensor-blas.git
    GIT_TAG 0.21.0  # Specify the required version
)
FetchContent_MakeAvailable(xtensor_blas)
