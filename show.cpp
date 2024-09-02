#include <igl/read_triangle_mesh.h>
#include <igl/decimate.h>
#include <igl/opengl/glfw/Viewer.h>

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <input_mesh.obj>" << std::endl;
        return 1;
    }

    std::string input_mesh_file = argv[1];

    Eigen::MatrixXd V, U;
    Eigen::MatrixXi F, G;
    Eigen::VectorXi J;

    if (!igl::read_triangle_mesh(input_mesh_file, V, F))
    {
        std::cerr << "Failed to read mesh file " << input_mesh_file << std::endl;
        return 1;
    }

    // Decimate the mesh
    igl::decimate(V, F, 10000, U, G, J);

    // print U and G sizes
    std::cout << "U: " << U.rows() << " " << U.cols() << std::endl;
    std::cout << "G: " << G.rows() << " " << G.cols() << std::endl;

    // Plot the mesh
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(U, G);
    viewer.data().set_face_based(true);
    viewer.launch();
}