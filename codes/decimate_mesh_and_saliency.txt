#include <igl/decimate.h>
#include <igl/read_triangle_mesh.h>
#include <igl/writeOBJ.h>
#include <igl/per_vertex_normals.h>
#include <igl/point_mesh_squared_distance.h>
#include <iostream>
#include <fstream>
#include <filesystem>

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cout << "Usage: " << argv[0] << " <input_mesh.obj> <input_face_saliency.txt> <output_folder> <max_m>" << std::endl;
        return 1;
    }

    std::string input_mesh_file = argv[1];
    std::string face_saliency_file = argv[2];
    std::string output_folder = argv[3];
    int max_m = std::stoi(argv[4]);

    // Ensure output directory exists
    if (!std::filesystem::exists(output_folder))
    {
        std::filesystem::create_directory(output_folder);
    }

    Eigen::MatrixXd V, U;
    Eigen::MatrixXi F, G;
    Eigen::VectorXi J;

    // Read the mesh from the file
    if (!igl::read_triangle_mesh(input_mesh_file, V, F))
    {
        std::cerr << "Failed to read mesh file " << input_mesh_file << std::endl;
        return 1;
    }

    // Read the face saliency values from the .txt file
    std::ifstream faceSaliencyFile(face_saliency_file);
    if (!faceSaliencyFile.is_open())
    {
        std::cerr << "Failed to open face saliency file " << face_saliency_file << std::endl;
        return 1;
    }

    Eigen::VectorXd faceSaliencyValues(F.rows());
    for (int i = 0; i < F.rows(); ++i)
    {
        if (!(faceSaliencyFile >> faceSaliencyValues(i)))
        {
            std::cerr << "Error reading face saliency value from " << face_saliency_file << std::endl;
            return 1;
        }
    }
    faceSaliencyFile.close();

    // Decimate the mesh
    igl::decimate(V, F, max_m, U, G, J);

    // Compute point-to-mesh squared distance
    Eigen::VectorXd sqrD;
    Eigen::VectorXi I;
    Eigen::MatrixXd C;
    igl::point_mesh_squared_distance(U, V, F, sqrD, I, C);

    // Prepare output filenames
    std::string base_name = std::filesystem::path(input_mesh_file).stem().string();
    std::string output_mesh_file = output_folder + "/" + base_name + "_decimated.obj";
    std::string output_face_saliency_file = output_folder + "/" + base_name + "_decimated.txt";

    // Save the decimated mesh
    if (!igl::writeOBJ(output_mesh_file, U, G))
    {
        std::cerr << "Failed to save decimated mesh to " << output_mesh_file << std::endl;
        return 1;
    }

    // Interpolate face saliency values
    Eigen::VectorXd decimatedFaceSaliencyValues(G.rows());
    for (int i = 0; i < G.rows(); ++i)
    {
        Eigen::RowVector3i face = G.row(i);
        double averageSaliency = 0.0;
        for (int j = 0; j < 3; ++j)
        {
            int originalIndex = I(face(j));
            if (originalIndex >= 0 && originalIndex < F.rows())
            {
                averageSaliency += faceSaliencyValues(originalIndex);
            }
        }
        decimatedFaceSaliencyValues(i) = averageSaliency / 3.0;
    }

    // Save the decimated face saliency values
    std::ofstream outFaceSaliencyFile(output_face_saliency_file);
    if (outFaceSaliencyFile.is_open())
    {
        for (int i = 0; i < decimatedFaceSaliencyValues.size(); ++i)
        {
            outFaceSaliencyFile << decimatedFaceSaliencyValues(i) << "\n";
        }
        outFaceSaliencyFile.close();
    }
    else
    {
        std::cerr << "Failed to open output face saliency file " << output_face_saliency_file << " for writing." << std::endl;
        return 1;
    }

    return 0;
}
