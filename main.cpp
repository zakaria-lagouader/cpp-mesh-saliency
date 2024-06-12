#include <iostream>
#include <fstream>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xnpy.hpp>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_npy_file>" << std::endl;
        return 1;
    }

    try
    {
        xt::xarray<int64_t> data = xt::load_npy<int64_t>(argv[1]);
        std::cout << data << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading .npy file: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
