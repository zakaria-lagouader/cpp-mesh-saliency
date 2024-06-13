#include <igl/read_triangle_mesh.h>
#include <igl/triangle_triangle_adjacency.h>
#include <Eigen/Core>
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <filesystem>
#include <sstream>

// Function to compute exactly n neighboring triangles
std::vector<int> computeNeighbors(const Eigen::MatrixXi &TT, int triangleIndex, int n)
{
    std::vector<int> neighbors;
    neighbors.reserve(n);
    std::queue<int> toVisit;
    std::vector<bool> visited(TT.rows(), false);

    toVisit.push(triangleIndex);
    visited[triangleIndex] = true;

    while (!toVisit.empty() && neighbors.size() < n)
    {
        int currentTriangle = toVisit.front();
        toVisit.pop();

        for (int j = 0; j < 3; ++j)
        {
            int neighbor = TT(currentTriangle, j);
            if (neighbor != -1 && !visited[neighbor])
            {
                visited[neighbor] = true;
                neighbors.push_back(neighbor);
                toVisit.push(neighbor);
                if (neighbors.size() == n)
                    break;
            }
        }
    }

    return neighbors;
}

void processTriangles(const Eigen::MatrixXi &TT, int startIdx, int endIdx, int n, std::ostringstream &stream)
{
    for (int i = startIdx; i < endIdx; ++i)
    {
        std::vector<int> neighbors = computeNeighbors(TT, i, n);

        stream << i;
        for (int neighbor : neighbors)
        {
            stream << "," << neighbor;
        }
        stream << "\n";
    }
}

void saveNeighborsToCSV(const Eigen::MatrixXi &F, const std::string &meshPath, const std::string &outputFolderPath, int n, int numThreads)
{
    std::filesystem::path meshFilePath(meshPath);
    std::string baseName = meshFilePath.stem().string();
    std::string outputFilePath = outputFolderPath + "/" + baseName + "_neighbors.csv";

    std::ofstream file(outputFilePath);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << outputFilePath << std::endl;
        return;
    }

    Eigen::MatrixXi TT, TTi;
    igl::triangle_triangle_adjacency(F, TT, TTi);

    int numTriangles = F.rows();
    int chunkSize = (numTriangles + numThreads - 1) / numThreads;

    std::vector<std::thread> threads;
    std::vector<std::ostringstream> streams(numThreads);

    for (int i = 0; i < numThreads; ++i)
    {
        int startIdx = i * chunkSize;
        int endIdx = (std::min)(startIdx + chunkSize, numTriangles);
        if (startIdx >= numTriangles)
            break;
        threads.emplace_back(processTriangles, std::ref(TT), startIdx, endIdx, n, std::ref(streams[i]));
    }

    for (std::thread &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    for (const auto &stream : streams)
    {
        file << stream.str();
    }

    file.close();
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cerr << "Usage: " << argv[0] << " <mesh_path> <output_folder_path> <n_neighbors> <num_threads>" << std::endl;
        return -1;
    }

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

    std::string meshPath = argv[1];
    std::string outputFolderPath = argv[2];
    int n = std::stoi(argv[3]);
    int numThreads = std::stoi(argv[4]);

    if (!igl::read_triangle_mesh(meshPath, V, F))
    {
        std::cerr << "Failed to read mesh from file: " << meshPath << std::endl;
        return -1;
    }

    saveNeighborsToCSV(F, meshPath, outputFolderPath, n, numThreads);

    std::cout << "Neighbors saved to: " << outputFolderPath << std::endl;

    return 0;
}
