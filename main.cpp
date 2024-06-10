#include <igl/decimate.h>
#include <igl/read_triangle_mesh.h>
#include <igl/writeOBJ.h>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    std::cout << "Usage: " << argv[0] << " <input_folder> <output_folder> <max_m>" << std::endl;
    return 1;
  }

  std::string input_folder = argv[1];
  std::string output_folder = argv[2];
  int max_m = std::stoi(argv[3]);

  // Ensure output directory exists
  if (!fs::exists(output_folder))
  {
    fs::create_directory(output_folder);
  }

  for (const auto &entry : fs::directory_iterator(input_folder))
  {
    if (entry.path().extension() == ".obj")
    {
      std::string mesh_file = entry.path().string();
      std::string saliency_file = mesh_file.substr(0, mesh_file.find_last_of(".")) + ".txt";
      std::string base_name = entry.path().stem().string();

      if (!fs::exists(saliency_file))
      {
        std::cerr << "Saliency file " << saliency_file << " does not exist, skipping." << std::endl;
        continue;
      }

      Eigen::MatrixXd V, U;
      Eigen::MatrixXi F, G;
      Eigen::VectorXi J;

      // Read the mesh from the file
      if (!igl::read_triangle_mesh(mesh_file, V, F))
      {
        std::cerr << "Failed to read mesh file " << mesh_file << std::endl;
        continue;
      }

      // Read the saliency values from the .txt file
      std::ifstream saliencyFile(saliency_file);
      if (!saliencyFile.is_open())
      {
        std::cerr << "Failed to open saliency file " << saliency_file << std::endl;
        continue;
      }

      Eigen::VectorXd saliencyValues(F.rows());
      for (int i = 0; i < F.rows(); ++i)
      {
        if (!(saliencyFile >> saliencyValues(i)))
        {
          std::cerr << "Error reading saliency value from " << saliency_file << std::endl;
          continue;
        }
      }
      saliencyFile.close();

      // Decimate the mesh
      igl::decimate(V, F, max_m, U, G, J);

      // Map the saliency values to the decimated mesh
      Eigen::VectorXd decimatedSaliencyValues(G.rows());
      for (int i = 0; i < G.rows(); ++i)
      {
        decimatedSaliencyValues(i) = saliencyValues(J(i));
      }

      // Save the decimated mesh
      std::string output_mesh_file = output_folder + "/" + base_name + "_decimated.obj";
      if (!igl::writeOBJ(output_mesh_file, U, G))
      {
        std::cerr << "Failed to save decimated mesh to " << output_mesh_file << std::endl;
      }

      // Save the decimated saliency values
      std::string output_saliency_file = output_folder + "/" + base_name + "_decimated.txt";
      std::ofstream outFile(output_saliency_file);
      if (outFile.is_open())
      {
        for (int i = 0; i < decimatedSaliencyValues.size(); ++i)
        {
          outFile << decimatedSaliencyValues(i) << "\n";
        }
        outFile.close();
      }
      else
      {
        std::cerr << "Failed to open output saliency file " << output_saliency_file << " for writing." << std::endl;
      }
    }
  }

  return 0;
}
