#include <igl/read_triangle_mesh.h>
#include <igl/triangle_triangle_adjacency.h>
#include <Eigen/Core>
#include <vector>
#include <queue>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <filesystem>

// Function to compute exactly n neighboring triangles
std::unordered_set<int> computeNeighbors(const Eigen::MatrixXi &F, int triangleIndex, int n)
{
  // Compute triangle-triangle adjacency
  Eigen::MatrixXi TT, TTi;
  igl::triangle_triangle_adjacency(F, TT, TTi);

  // Set to store visited triangles
  std::unordered_set<int> visited;
  // Queue for BFS
  std::queue<int> toVisit;

  // Start with the initial triangle
  toVisit.push(triangleIndex);
  visited.insert(triangleIndex);

  while (!toVisit.empty() && visited.size() <= n)
  {
    int currentTriangle = toVisit.front();
    toVisit.pop();

    // Iterate over the neighbors of the current triangle
    for (int j = 0; j < 3; ++j)
    {
      int neighbor = TT(currentTriangle, j);
      if (neighbor != -1 && visited.find(neighbor) == visited.end())
      {
        visited.insert(neighbor);
        toVisit.push(neighbor);
        // Stop once we have exactly n neighbors
        if (visited.size() == n + 1)
        {
          visited.erase(triangleIndex);
          return visited;
        }
      }
    }
  }

  // Remove the initial triangle from the result set if not already done
  visited.erase(triangleIndex);
  return visited;
}

void saveNeighborsToCSV(const Eigen::MatrixXi &F, const std::string &meshPath, const std::string &outputFolderPath, int n)
{
  // Extract the base name of the mesh file
  std::filesystem::path meshFilePath(meshPath);
  std::string baseName = meshFilePath.stem().string();
  std::string outputFilePath = outputFolderPath + "/" + baseName + "_neighbors.csv";

  std::ofstream file(outputFilePath);
  if (!file.is_open())
  {
    std::cerr << "Unable to open file: " << outputFilePath << std::endl;
    return;
  }

  for (int i = 0; i < F.rows(); ++i)
  {
    std::unordered_set<int> neighbors = computeNeighbors(F, i, n);
    // Write the triangle index and its neighbors to the CSV file
    file << i;
    for (int neighbor : neighbors)
    {
      file << "," << neighbor;
    }
    file << "\n";
  }

  file.close();
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    std::cerr << "Usage: " << argv[0] << " <mesh_path> <output_folder_path> <n_neighbors>" << std::endl;
    return -1;
  }

  Eigen::MatrixXd V;
  Eigen::MatrixXi F;

  // Read mesh from file (provide path to your mesh file)
  std::string meshPath = argv[1];
  std::string outputFolderPath = argv[2];
  int n = std::stoi(argv[3]);

  if (!igl::read_triangle_mesh(meshPath, V, F))
  {
    std::cerr << "Failed to read mesh from file: " << meshPath << std::endl;
    return -1;
  }

  // Save neighbors to CSV
  saveNeighborsToCSV(F, meshPath, outputFolderPath, n);

  return 0;
}
