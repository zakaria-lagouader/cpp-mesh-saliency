#include <iostream>
#include <fstream>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xnpy.hpp>
#include <xtensor/xcsv.hpp>

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_face_neighbors> <path_to_normals> <path_to_vars>" << std::endl;
        return 1;
    }

    std::string face_neighbors_path = argv[1];
    std::string face_normals_path = argv[2];
    std::string vars_path = argv[3];

    std::ifstream face_neighbors_file;
    face_neighbors_file.open(face_neighbors_path);
    std::ifstream face_normals_file;
    face_normals_file.open(face_normals_path);

    if (!face_neighbors_file.is_open())
    {
        std::cerr << "Error opening file: " << face_neighbors_path << std::endl;
        return 1;
    }

    if (!face_normals_file.is_open())
    {
        std::cerr << "Error opening file: " << face_normals_path << std::endl;
        return 1;
    }

    try
    {
        xt::xarray<int64_t> HC2I = xt::load_npy<int64_t>(vars_path + "/HC2I.npy");
        xt::xarray<int64_t> I2HC = xt::load_npy<int64_t>(vars_path + "/I2HC.npy");
        xt::xarray<int> face_neighbors = xt::load_csv<int>(face_neighbors_file);
        xt::xarray<int> face_normals = xt::load_csv<int>(face_normals_file);

        std::cout << "HC2I shape: " << xt::adapt(HC2I.shape()) << std::endl;
        std::cout << "I2HC shape: " << xt::adapt(I2HC.shape()) << std::endl;
        std::cout << "face_neighbors shape: " << xt::adapt(face_neighbors.shape()) << std::endl;
        std::cout << "face_normals shape: " << xt::adapt(face_normals.shape()) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading files: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
