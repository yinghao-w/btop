#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Minimal NVML types ---
typedef int nvmlReturn_t;
typedef void *nvmlDevice_t;

#define NVML_SUCCESS 0
#define NVML_ERROR_INVALID_ARGUMENT 1

typedef struct {
  unsigned int gpu;
  unsigned int memory;
} nvmlUtilization_t;

typedef struct {
  uint64_t total;
  uint64_t free;
  uint64_t used;
} nvmlMemory_t;

typedef enum { NVML_TEMPERATURE_GPU = 0 } nvmlTemperatureSensors_t;

// --- Fake GPU data ---
typedef struct {
  char name[64];
  char uuid[64];
  char pci_bus[16];
  uint64_t mem_total;
  uint64_t mem_used;
  unsigned int util_gpu;
  unsigned int temperature;
} FakeGpu;

#define MAX_GPUS 16
static FakeGpu gpus[MAX_GPUS];
static unsigned int num_gpus = 3;

// --- Hard-coded 3 fake GPUs ---
void init_gpus() {
  strcpy(gpus[0].name, "NVIDIA RTX 4090");
  strcpy(gpus[0].uuid, "GPU-4090-FAKE-001");
  strcpy(gpus[0].pci_bus, "0000:01:00.0");
  gpus[0].mem_total = 24ULL * 1024 * 1024 * 1024;
  gpus[0].mem_used = 4ULL * 1024 * 1024 * 1024;
  gpus[0].util_gpu = 0;
  gpus[0].temperature = 35;

  strcpy(gpus[1].name, "NVIDIA RTX 4080");
  strcpy(gpus[1].uuid, "GPU-4080-FAKE-002");
  strcpy(gpus[1].pci_bus, "0000:02:00.0");
  gpus[1].mem_total = 16ULL * 1024 * 1024 * 1024;
  gpus[1].mem_used = 2ULL * 1024 * 1024 * 1024;
  gpus[1].util_gpu = 0;
  gpus[1].temperature = 33;

  strcpy(gpus[2].name, "NVIDIA RTX 4070 Ti");
  strcpy(gpus[2].uuid, "GPU-4070TI-FAKE-003");
  strcpy(gpus[2].pci_bus, "0000:03:00.0");
  gpus[2].mem_total = 12ULL * 1024 * 1024 * 1024;
  gpus[2].mem_used = 1ULL * 1024 * 1024 * 1024;
  gpus[2].util_gpu = 0;
  gpus[2].temperature = 32;
}

// --- NVML API functions ---
nvmlReturn_t nvmlInit() {
  srand(time(NULL));
  init_gpus();
  return NVML_SUCCESS;
}

nvmlReturn_t nvmlShutdown() { return NVML_SUCCESS; }

nvmlReturn_t nvmlDeviceGetCount(unsigned int *count) {
  *count = num_gpus;
  return NVML_SUCCESS;
}

nvmlReturn_t nvmlDeviceGetHandleByIndex(unsigned int index,
                                        nvmlDevice_t *device) {
  if (index >= num_gpus)
    return NVML_ERROR_INVALID_ARGUMENT;
  *device = (nvmlDevice_t)(uintptr_t)(index + 1);
  return NVML_SUCCESS;
}

nvmlReturn_t nvmlDeviceGetName(nvmlDevice_t device, char *name,
                               unsigned int length) {
  int index = (int)(uintptr_t)device - 1;
  strncpy(name, gpus[index].name, length - 1);
  name[length - 1] = 0;
  return NVML_SUCCESS;
}

nvmlReturn_t nvmlDeviceGetUUID(nvmlDevice_t device, char *uuid,
                               unsigned int length) {
  int index = (int)(uintptr_t)device - 1;
  strncpy(uuid, gpus[index].uuid, length - 1);
  uuid[length - 1] = 0;
  return NVML_SUCCESS;
}

nvmlReturn_t nvmlDeviceGetMemoryInfo(nvmlDevice_t device, nvmlMemory_t *info) {
  int index = (int)(uintptr_t)device - 1;
  info->total = gpus[index].mem_total;
  info->used = gpus[index].mem_used;
  info->free = info->total - info->used;
  return NVML_SUCCESS;
}

nvmlReturn_t nvmlDeviceGetUtilizationRates(nvmlDevice_t device,
                                           nvmlUtilization_t *util) {
  int index = (int)(uintptr_t)device - 1;
  util->gpu = rand() % 100;
  util->memory = rand() % 100;
  return NVML_SUCCESS;
}

nvmlReturn_t nvmlDeviceGetTemperature(nvmlDevice_t device,
                                      nvmlTemperatureSensors_t sensorType,
                                      unsigned int *temp) {
  int index = (int)(uintptr_t)device - 1;
  *temp = gpus[index].temperature + rand() % 5; // small variance
  return NVML_SUCCESS;
}

char nvmlErrorMessage[] = "This is a mock nvml error message";

const char *nvmlErrorString(nvmlReturn_t param) { return nvmlErrorMessage; }

nvmlReturn_t nvmlDeviceGetPowerManagementLimit(nvmlDevice_t device,
                                               unsigned int *limit) {
  *limit = 222;
  return NVML_SUCCESS;
}

typedef int nvmlTemperatureThresholds_t;

nvmlReturn_t
nvmlDeviceGetTemperatureThreshold(nvmlDevice_t device,
                                  nvmlTemperatureThresholds_t param1,
                                  unsigned int *param2) {
  *param2 = 111;
  return NVML_SUCCESS;
}

typedef int nvmlClockType_t;

nvmlReturn_t nvmlDeviceGetClockInfo(nvmlDevice_t, nvmlClockType_t param1,
                                    unsigned int *param2) {
  *param2 = 1111;
  return NVML_SUCCESS;
}

nvmlReturn_t nvmlDeviceGetPowerUsage(nvmlDevice_t, unsigned int *param) {
  *param = 111111;
  return NVML_SUCCESS;
}

typedef int nvmlPstates_t;

nvmlReturn_t nvmlDeviceGetPowerState(nvmlDevice_t, nvmlPstates_t *param) {
  *param = 4;
  return NVML_SUCCESS;
}

typedef int nvmlPcieUtilCounter_t;

nvmlReturn_t nvmlDeviceGetPcieThroughput(nvmlDevice_t, nvmlPcieUtilCounter_t,
                                         unsigned int *param) {
  *param = 4444;
  return NVML_SUCCESS;
}
nvmlReturn_t nvmlDeviceGetEncoderUtilization(nvmlDevice_t, unsigned int *param1,
                                             unsigned int *) {
  *param1 = 3;
  return NVML_SUCCESS;
}
nvmlReturn_t nvmlDeviceGetDecoderUtilization(nvmlDevice_t, unsigned int *param1,
                                             unsigned int *) {
  *param1 = 3;
  return NVML_SUCCESS;
}
