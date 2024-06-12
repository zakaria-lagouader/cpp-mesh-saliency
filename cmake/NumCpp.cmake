if(TARGET NumCpp)
    return()
endif()

include(FetchContent)
FetchContent_Declare(
    NumCpp
    GIT_REPOSITORY https://github.com/dpilger26/NumCpp
    GIT_TAG Version_2.12.1)
FetchContent_MakeAvailable(NumCpp)
