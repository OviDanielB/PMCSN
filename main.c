
#include "utils/constants.h"
#include "event_time_generator.h"


int main() {


    /* plants a seed for the stream generator (automatically generates seed for every stream) */
    PlantSeeds(SEED);

    printf("Arrival Time Class 1 : %.2f \n", getArrivalClass1());
    printf("Arrival Time Class 2 : %.2f \n", getArrivalClass2());

    return 0;
}