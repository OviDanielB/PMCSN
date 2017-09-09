#include <errno.h>
#include "event_time_generator.h"
#include "output.h"


/**
 * Represents the number of tasks
 * in each part of the system
 */
struct {
    long cldlet_1;
    long cldlet_2;
    long cloud_1;
    long cloud_2;
    long setup_2;
} state = {0, 0, 0, 0, 0};

/**
 * Keeps track of simulation time
 */
struct {
    double current;
    double next;
} time = {0.0, 0.0};

int current_batch = 0;                   // batch in execution
double batch_end;
double simulation_end;

/**
 * initialize first arrival time events
 */
void init_arrival() {

    create_and_insert_event(EVENT_CLASS_1_ARRIVAL, getArrivalClass1());
    create_and_insert_event(EVENT_CLASS_2_ARRIVAL, getArrivalClass2());
}

/**
 * convert to int
 * @param argv program arguments
 * @param arg_index
 * @return integer
 */
int read_int(char **argv, int arg_index) {

    errno = 0;
    int v = (int) strtol(argv[arg_index], NULL, 10);
    if (errno != 0) {
        fprintf(stderr, "Usage %s <N> <S> <#Batch> <Batch size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return v;
}

void init_params(int argc, char **argv) {

    if (argc != 5) {
        fprintf(stderr, "Usage %s <N> <S> <#Batch> <Batch size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    N = read_int(argv, 1);
    S = read_int(argv, 2);
    batch_number = read_int(argv, 3);
    batch_time = read_int(argv, 4);

    simulation_end = (batch_number) * batch_time;


}

/**
 * get number of jobs still processing
 */
long jobs_left() {
    return state.cldlet_1 + state.cldlet_2 + state.cloud_1 + state.cloud_2 + state.setup_2;
}

int is_arrival(struct event *pEvent) {

    return pEvent->type == EVENT_CLASS_1_ARRIVAL || pEvent->type == EVENT_CLASS_2_ARRIVAL ||
           pEvent->type == EVENT_CLASS_2_SETUP;

}

/**
 * given an arrival, return the action
 * to take (send on cloud, leave in cloudlet, etc)
 * @param event
 * @return action
 */
int dispatch(struct event *event) {
    switch (event->type) {
        case EVENT_CLASS_1_ARRIVAL:
            if (state.cldlet_1 == N) {
                return SEND_CLASS_1_TO_CLOUD;
            } else {
                if (state.cldlet_1 + state.cldlet_2 < S) {
                    return ACCEPT_CLASS_1_ON_CLOUDLET;
                } else {
                    if (state.cldlet_2 > 0)
                        return INTERRUPT_CLASS_2_ON_CLOUDLET_AND_SEND_TO_CLOUD;
                    else
                        return ACCEPT_CLASS_1_ON_CLOUDLET;

                }
            }

        case EVENT_CLASS_2_ARRIVAL:
            if (state.cldlet_1 + state.cldlet_2 >= S)
                return SEND_CLASS_2_TO_CLOUD;
            else
                return ACCEPT_CLASS_2_ON_CLOUDLET;
        default:
            fprintf(stderr, "Event type should be an arrival!\n");
            exit(EXIT_FAILURE);

    }
}

void next_arrival(int class_event) {

    if (time.current >= simulation_end) {
        return;
    }
    switch (class_event) {
        case EVENT_CLASS_1_ARRIVAL:
            create_and_insert_event(EVENT_CLASS_1_ARRIVAL, getArrivalClass1());
            break;
        case EVENT_CLASS_2_ARRIVAL:
            create_and_insert_event(EVENT_CLASS_2_ARRIVAL, getArrivalClass2());
            break;
        default:
            fprintf(stderr, "Not an arrival\n");
    }
}

void update_node_area() {

    double interval = time.next - time.current;
    area[current_batch].node += interval * (jobs_left());
    area[current_batch].cloud_node += interval * (state.cloud_1 + state.cloud_2);
    area[current_batch].cloudlet_node += interval * (state.cldlet_1 + state.cldlet_2);

}

void execute_arrival(struct event *arrival_event, int action) {

    time.next = arrival_event->time;
    update_node_area();
    time.current = time.next;

    switch (action) {
        case SEND_CLASS_1_TO_CLOUD:
            state.cloud_1++;

            create_and_insert_event(EVENT_CLASS_1_CLOUD_COMPLETION, time.current + getServiceClass1Cloud());

            next_arrival(EVENT_CLASS_1_ARRIVAL);
            break;
        case ACCEPT_CLASS_1_ON_CLOUDLET:
            state.cldlet_1++;

            create_and_insert_event(EVENT_CLASS_1_CLOUDLET_COMPLETION, time.current + getServiceClass1Cloudlet());

            next_arrival(EVENT_CLASS_1_ARRIVAL);
            break;
        case INTERRUPT_CLASS_2_ON_CLOUDLET_AND_SEND_TO_CLOUD:
            state.cldlet_1++;
            state.cldlet_2--;
            state.cloud_2++;
            completed->interrupted_class_2++;

            /* compute remaining service time for job of class 2 to interrupt */
            struct event *class_2_event_cloudlet = remove_first_event_by_type(EVENT_CLASS_2_CLOUDLET_COMPLETION);
            double remaining_time = class_2_event_cloudlet->time - time.current;
            double job_remaining_percentage = remaining_time / class_2_event_cloudlet->job_size;

            /* update service time spent for interrupted jobs */
            double wasted_time = time.current - (class_2_event_cloudlet->time - class_2_event_cloudlet->job_size);
            service->cloudlet_class_2_interrupted += wasted_time;

            /* compute new service time for interrupted job in the cloud */
            double class_2_cloud_service_time = getServiceClass2Cloud();
            double class_2_cloud_remaining_time = class_2_cloud_service_time * job_remaining_percentage;
            double service_time = class_2_cloud_remaining_time + getSetup();
            service->cloud_class_2_interrupted += service_time;
            create_and_insert_event(EVENT_CLASS_2_CLOUD_COMPLETION, time.current + service_time);

            /*For the job just entered that caused the class 2 job's interruption */
            create_and_insert_event(EVENT_CLASS_1_CLOUDLET_COMPLETION, time.current + getServiceClass1Cloudlet());

            break;
        case SEND_CLASS_2_TO_CLOUD:
            state.cloud_2++;

            create_and_insert_event(EVENT_CLASS_2_CLOUD_COMPLETION, time.current + getServiceClass2Cloud());

            next_arrival(EVENT_CLASS_2_ARRIVAL);
            break;
        case ACCEPT_CLASS_2_ON_CLOUDLET:
            state.cldlet_2++;

            struct event *cl_2_event = create_and_insert_event(EVENT_CLASS_2_CLOUDLET_COMPLETION,
                                                               time.current + getServiceClass2Cloudlet());
            cl_2_event->job_size = cl_2_event->time - arrival_event->time;

            next_arrival(EVENT_CLASS_2_ARRIVAL);
            break;
        default:
            fprintf(stderr, "No action matching!\n");
    }


}

void execute_completion(struct event *event) {

    time.current = event->time;

    switch (event->type) {
        case EVENT_CLASS_1_CLOUDLET_COMPLETION:
            state.cldlet_1--;
            completed[current_batch].cloudlet_class_1++;
            service[current_batch].cloudlet_class_1 += event->job_size;

            break;
        case EVENT_CLASS_2_CLOUDLET_COMPLETION:
            state.cldlet_2--;
            completed[current_batch].cloudlet_class_2++;
            service[current_batch].cloudlet_class_2 += event->job_size;

            break;
        case EVENT_CLASS_1_CLOUD_COMPLETION:
            state.cloud_1--;
            completed[current_batch].cloud_class_1++;
            service[current_batch].cloud_class_1 += event->job_size;

            break;
        case EVENT_CLASS_2_CLOUD_COMPLETION:
            state.cloud_2--;
            completed[current_batch].cloud_class_2++;
            service[current_batch].cloud_class_2 += event->job_size;

            break;
        default:
            fprintf(stderr, "No completion type matching!\n");
    }
}

void process_event(struct event *event) {

    int dispatch_action;
    if (is_arrival(event)) {
        dispatch_action = dispatch(event);
        execute_arrival(event, dispatch_action);
    } else { /* completion */
        execute_completion(event);
    }
}

int main(int argc, char **argv) {


    init_params(argc, argv);
    /* plants a seed for the stream generator (automatically generates seed for every stream) */
    PlantSeeds(SEED);
    init_arrival();
    init_output_stats();


    for (current_batch = 0; current_batch < batch_number; current_batch++) {
        batch_end = (current_batch + 1) * batch_time;
        while (time.current < batch_end || (current_batch == batch_number - 1 && jobs_left() != 0)) {
            process_event(pop_event());
        }

        /* Warning: The denominator must be time.current and NOT batch_end for computation correctness*/
        batch_stat[current_batch].avg_node = area->node / time.current;
        batch_stat[current_batch].avg_node_cloudlet = area->cloudlet_node / time.current;
        batch_stat[current_batch].avg_node_cloud = area->cloud_node / time.current;

        //TODO compute probabilities
    }

    //TODO compute mean and std of batch means
    //TODO compute throughput


    return 0;
}