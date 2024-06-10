#include <igl/read_triangle_mesh.h>
#include <igl/triangle_triangle_adjacency.h>
#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>
#include <vector>
#include <queue>
#include <unordered_set>

// Function to compute exactly n neighboring triangles
std::unordered_set<int> computeNeighbors(
    const Eigen::MatrixXi &F, int triangleIndex, int n)
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

int main(int argc, char *argv[])
{
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;

  // Read mesh from file (provide path to your mesh file)
  igl::read_triangle_mesh(argv[1], V, F);

  // Example usage
  int triangleIndex = 0;      // The starting triangle index
  int n = std::stoi(argv[2]); // The number of neighboring triangles
  std::unordered_set<int> neighbors = computeNeighbors(F, triangleIndex, n);

  // add the starting triangle to the neighbors set
  neighbors.insert(triangleIndex);

  // Create a color matrix for faces
  Eigen::MatrixXd faceColors = Eigen::MatrixXd::Constant(F.rows(), 3, 1.0); // Default to white

  // Set the color of the neighboring faces to red
  for (int tri : neighbors)
  {
    faceColors(tri, 0) = 1.0; // Red
    faceColors(tri, 1) = 0.0; // Green
    faceColors(tri, 2) = 0.0; // Blue
  }

  // Create a viewer
  igl::opengl::glfw::Viewer viewer;

  // Set the mesh
  viewer.data().set_mesh(V, F);

  // Set the face colors
  viewer.data().set_colors(faceColors);

  // Launch the viewer
  viewer.launch();

  return 0;
}
