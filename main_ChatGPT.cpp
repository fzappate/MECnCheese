#include <iostream>
#include <vector>
#include <sundials/sundials_types.h>
#include <nvector/nvector_serial.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <cvode/cvode.h>
#include <cvode/cvode_direct.h>
#include <sunmatrix/sunmatrix_dense.h>

// Function to compute the ODE system
int massSpringDamper(realtype t, N_Vector y, N_Vector ydot, void *user_data) {
    realtype *ydata = N_VGetArrayPointer(y);
    realtype *ydotdata = N_VGetArrayPointer(ydot);

    // Constants
    realtype mass = 1.0;
    realtype damping = 0.5;
    realtype stiffness = 10.0;

    // Extracting state variables
    realtype position = ydata[0];
    realtype velocity = ydata[1];

    // ODEs
    ydotdata[0] = velocity;
    ydotdata[1] = (-damping * velocity - stiffness * position) / mass;

    return 0;
}

int main() {
    // Initialize vectors
    N_Vector y = N_VNew_Serial(2);
    N_Vector abstol = N_VNew_Serial(2);

    // Initialize CVODE solver
    void *cvode_mem = CVodeCreate(CV_ADAMS, CV_FUNCTIONAL);
    CVodeInit(cvode_mem, massSpringDamper, 0.0, y);
    CVodeSStolerances(cvode_mem, 1e-6, 1e-6);
    CVodeSetUserData(cvode_mem, nullptr);

    // Set initial conditions
    NV_Ith_S(y, 0) = 1.0;  // initial position
    NV_Ith_S(y, 1) = 0.0;  // initial velocity

    // Integrate over time
    realtype t_end = 10.0;
    realtype t_out = 0.1;
    while (t_out <= t_end) {
        CVode(cvode_mem, t_out, y, nullptr, CV_NORMAL);
        std::cout << "Time: " << t_out << ", Position: " << NV_Ith_S(y, 0) << ", Velocity: " << NV_Ith_S(y, 1) << std::endl;
        t_out += 0.1;
    }

    // Free memory
    N_VDestroy_Serial(y);
    N_VDestroy_Serial(abstol);
    CVodeFree(&cvode_mem);

    return 0;
}
