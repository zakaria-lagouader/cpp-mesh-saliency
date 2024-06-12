#include <igl/read_triangle_mesh.h>
#include <igl/per_face_normals.h>
#include <igl/doublearea.h>
#include <Eigen/Core>
#include <fstream>
#include <iostream>
#include <filesystem>

void saveFaceNormalsToCSV(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F, const std::string &outputFilePath)
{
    Eigen::MatrixXd normals;
    igl::per_face_normals(V, F, normals);

    std::ofstream file(outputFilePath);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << outputFilePath << std::endl;
        return;
    }

    for (int i = 0; i < normals.rows(); ++i)
    {
        file << normals(i, 0) << "," << normals(i, 1) << "," << normals(i, 2) << "\n";
    }

    file.close();
}

void saveFaceAreasToCSV(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F, const std::string &outputFilePath)
{
    Eigen::VectorXd doubleArea;
    igl::doublearea(V, F, doubleArea);

    std::ofstream file(outputFilePath);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << outputFilePath << std::endl;
        return;
    }

    for (int i = 0; i < doubleArea.size(); ++i)
    {
        file << doubleArea(i) / 2.0 << "\n"; // Area is half of the double area
    }

    file.close();
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <mesh_path> <output_folder_path>" << std::endl;
        return -1;
    }

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

    // Read mesh from file (provide path to your mesh file)
    std::string meshPath = argv[1];
    std::string outputFolderPath = argv[2];

    if (!igl::read_triangle_mesh(meshPath, V, F))
    {
        std::cerr << "Failed to read mesh from file: " << meshPath << std::endl;
        return -1;
    }

    // Extract the base name of the mesh file
    std::filesystem::path meshFilePath(meshPath);
    std::string baseName = meshFilePath.stem().string();

    // Save face normals to CSV
    std::string normalsFilePath = outputFolderPath + "/" + baseName + "_face_normals.csv";
    saveFaceNormalsToCSV(V, F, normalsFilePath);

    // Save face areas to CSV
    std::string areasFilePath = outputFolderPath + "/" + baseName + "_face_area.csv";
    saveFaceAreasToCSV(V, F, areasFilePath);

    std::cout << "Face normals and areas saved to: " << outputFolderPath << std::endl;

    return 0;
}
