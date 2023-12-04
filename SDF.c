#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Define the structure for sensor data fusion
struct SDF {
    float temperature;
    float speed;
    float pressure;
    float fused;
};

typedef struct SDF *SDFP;

// function declaration
struct SDF input();

int main() {
    struct SDF sdata;
    SDFP shared_memory;

    int shmid, key_t = 23452;
    // Create or get the shared memory segment
    shmid = shmget(key_t, 1024, 0666 | IPC_CREAT);
    // Attach the shared memory segment
    shared_memory = (SDFP)shmat(shmid, NULL, 0);

    if (shmid == -1) {
        printf("\nSHARED MEMORY NOT ASSIGNED\n");
        exit(0);
    }
    // function call
    sdata = input();
    // copy the sensor data to shared memory
    *shared_memory = sdata;

    // display the sensor data
printf("\nSensor DATA\nTemperature:%.2f\nSpeed:%.2f\nPressure:%.2f\nFused:%.2f\n",shared_memory->temperature, shared_memory->speed,shared_memory->pressure, shared_memory->fused);
}
// Function to input sensor data and perform sensor data fusion
struct SDF input() {
    struct SDF avg;

    float input;
    float temp, pressure, speed;
    // assigning weights as per algorithm
    float w_temp = 0.4; // w_temp is weight assigned to temperature
    float w_pressure = 0.3; // w_pressure is weight assigned to pressure
    float w_speed = 0.3;// w_speed is weight assigned to speed

    int n = 3;
    srand(time(0));
    for (int i = 1; i <= n; i++) {
        printf("----------------------------------------\n");
        int min = 180, max = 220;

        temp = rand() % (max - min + 1) + min;
        printf("\nTHE TEMPERATURE (in Celsius(C)) (>180)\n");
        printf("Temperature: %.2f\n", temp);
        if (temp < 180) {
            printf("\nINVALID INPUT\n");
        }
        // pmin is minimum pressure and pmax is maximum pressure
        int pmin = 25, pmax = 65;
        pressure = rand() % (pmax - pmin + 1) + pmin;
        printf("\nTHE PRESSURE (in KiloPascals(kPA)) (25-65)\n");
        printf("Pressure:%.2f", pressure);

        if (pressure < 25 || pressure > 65) {
            printf("\nINVALID INPUT\n");
        }
        //smin is minimum speed and smax is maximum speed
        int smin = 0, smax = 4000;
        speed = rand() % (smax - smin + 1) + smin;
        printf("\nTHE SPEED (IN RPM) (0-4000)\n");
        printf("Speed: %.2f\n", speed);

        if (speed < 0 || speed > 4000) {
            printf("\nINVALID INPUT\n");
        }

        avg.temperature += temp;
        avg.pressure += pressure;
        avg.speed += speed;
    }
    // Calculate average values
    avg.temperature /= n;  // average temperature
    avg.pressure /= n;    // average pressure
    avg.speed /= n;       // average speed
    // Perform senor data fusion(weighted average method )
    avg.fused = (avg.temperature * w_temp) + (avg.pressure * w_pressure) +
                            (avg.speed * w_speed);

    return avg;
}
