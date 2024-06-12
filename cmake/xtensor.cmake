if(TARGET xtensor)
    return()
endif()

include(FetchContent)

# Fetch xtl dependency
FetchContent_Declare(
    xtl
    GIT_REPOSITORY https://github.com/xtensor-stack/xtl.git
    GIT_TAG 0.7.7  # You can specify the version you need
)
FetchContent_MakeAvailable(xtl)

# Fetch xtensor library
FetchContent_Declare(
    xtensor
    GIT_REPOSITORY https://github.com/xtensor-stack/xtensor.git
    GIT_TAG 0.25.0  # You can specify the version you need
)
FetchContent_MakeAvailable(xtensor)
