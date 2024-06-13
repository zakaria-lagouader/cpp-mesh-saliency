#include <igl/decimate.h>
#include <igl/read_triangle_mesh.h>
#include <igl/writeOBJ.h>
#include <fstream>
#include <filesystem>

int main(int argc, char *argv[])
{
    int max_m = 10000; // Default value

    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <mesh_path> <output_folder_path> [max_triangles]" << std::endl;
        return -1;
    }

    // Check if max_m is provided in argv
    if (argc == 4)
    {
        max_m = std::stoi(argv[3]);
    }

    Eigen::MatrixXd V, U;
    Eigen::MatrixXi F, G;
    Eigen::VectorXi J;

    // Read the mesh from the file
    igl::read_triangle_mesh(argv[1], V, F);

    // Decimate the mesh
    igl::decimate(V, F, max_m, U, G, J);

    // Get the input file name
    std::filesystem::path inputFilePath(argv[1]);
    std::string inputFileName = inputFilePath.stem().string();

    // Save the decimated mesh with the same name as the input file
    std::string outputFilePath = std::string(argv[2]) + "/" + inputFileName + "_decimated.obj";
    igl::writeOBJ(outputFilePath, U, G);

    std::cout << "Decimated mesh saved at: " << outputFilePath << std::endl;
}
