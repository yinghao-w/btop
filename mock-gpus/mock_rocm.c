// mock_rocm_smi.c — Mock librocm_smi64.so
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// ----------------------------
// Mock RSMI Type Definitions
// ----------------------------
typedef enum {
  RSMI_STATUS_SUCCESS = 0,
  RSMI_STATUS_NOT_SUPPORTED = 1,
  RSMI_STATUS_INVALID_ARGS = 2
} rsmi_status_t;

typedef struct {
  int major;
  int minor;
  int patch;
  char build[64];
} rsmi_version_t;

typedef enum { RSMI_TEMP_CURRENT = 0 } rsmi_temperature_metric_t;

typedef enum { RSMI_CLK_TYPE_SYS = 0, RSMI_CLK_TYPE_MEM = 1 } rsmi_clk_type_t;

typedef enum {
  RSMI_MEM_TYPE_VRAM = 0,
  RSMI_MEM_TYPE_VIS_VRAM = 1
} rsmi_memory_type_t;

typedef struct {
  uint32_t num_supported;
  uint32_t frequency[8];
} rsmi_frequencies_t_v5;

typedef struct {
  uint32_t num_supported;
  uint64_t frequency[8];
} rsmi_frequencies_t_v6;

// ----------------------------
// Hard-coded Mock GPU Data
// ----------------------------
typedef struct {
  const char *name;
  uint64_t power_cap;
  int64_t temperature;
  uint32_t busy_percent;
  uint32_t mem_busy_percent;
  uint64_t power_ave;
  uint64_t mem_total;
  uint64_t mem_used;
  uint64_t pci_sent;
  uint64_t pci_recv;
  uint64_t pci_max;
} mock_gpu_t;

static mock_gpu_t mock_gpus[2] = {
    {"Mock Radeon GPU 0",
     150000000ULL, // 150W
     65000,        // 65 C
     42,           // busy %
     55,           // mem busy %
     80000000ULL,  // 80W ave
     16ULL * 1024 * 1024 * 1024, 6ULL * 1024 * 1024 * 1024, 400000000ULL,
     350000000ULL, 800000000ULL},
    {"Mock Radeon GPU 1", 180000000ULL,
     72000, // 72 C
     77, 68, 120000000ULL, 24ULL * 1024 * 1024 * 1024,
     12ULL * 1024 * 1024 * 1024, 500000000ULL, 470000000ULL, 900000000ULL}};

// ----------------------------
// Mock RSMI Function Implementations
// ----------------------------
rsmi_status_t rsmi_init(uint64_t flags) {
  (void)flags;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_shut_down() { return RSMI_STATUS_SUCCESS; }

rsmi_status_t rsmi_version_get(rsmi_version_t *v) {
  if (!v)
    return RSMI_STATUS_INVALID_ARGS;
  v->major = 6;
  v->minor = 0;
  v->patch = 0;
  strcpy(v->build, "mock-build");
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_num_monitor_devices(uint32_t *num) {
  if (!num)
    return RSMI_STATUS_INVALID_ARGS;
  *num = 2;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_name_get(uint32_t id, char *name, size_t len) {
  if (id >= 2 || !name)
    return RSMI_STATUS_INVALID_ARGS;
  strncpy(name, mock_gpus[id].name, len - 1);
  name[len - 1] = '\0';
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_power_cap_get(uint32_t id, uint32_t sensor,
                                     uint64_t *cap) {
  (void)sensor;
  if (id >= 2 || !cap)
    return RSMI_STATUS_INVALID_ARGS;
  *cap = mock_gpus[id].power_cap;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_temp_metric_get(uint32_t id, uint32_t sensor,
                                       rsmi_temperature_metric_t metric,
                                       int64_t *temp) {
  (void)sensor;
  (void)metric;
  if (id >= 2 || !temp)
    return RSMI_STATUS_INVALID_ARGS;
  *temp = mock_gpus[id].temperature;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_busy_percent_get(uint32_t id, uint32_t *pct) {
  if (id >= 2 || !pct)
    return RSMI_STATUS_INVALID_ARGS;
  *pct = mock_gpus[id].busy_percent;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_memory_busy_percent_get(uint32_t id, uint32_t *pct) {
  if (id >= 2 || !pct)
    return RSMI_STATUS_INVALID_ARGS;
  *pct = mock_gpus[id].mem_busy_percent;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_gpu_clk_freq_get(uint32_t id, rsmi_clk_type_t clk,
                                        rsmi_frequencies_t_v6 *f) {
  if (id >= 2 || !f)
    return RSMI_STATUS_INVALID_ARGS;
  f->num_supported = 3;
  // f->frequency[0] = 500000000ULL;
  f->frequency[0] = 1LL;
  f->frequency[1] = 1000000000ULL;
  f->frequency[2] = 1500000000ULL;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_power_ave_get(uint32_t id, uint32_t sensor,
                                     uint64_t *pwr) {
  (void)sensor;
  if (id >= 2 || !pwr)
    return RSMI_STATUS_INVALID_ARGS;
  *pwr = mock_gpus[id].power_ave;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_memory_total_get(uint32_t id,
                                        rsmi_memory_type_t mem_type,
                                        uint64_t *bytes) {
  (void)mem_type;
  if (id >= 2 || !bytes)
    return RSMI_STATUS_INVALID_ARGS;
  *bytes = mock_gpus[id].mem_total;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_memory_usage_get(uint32_t id,
                                        rsmi_memory_type_t mem_type,
                                        uint64_t *bytes) {
  (void)mem_type;
  if (id >= 2 || !bytes)
    return RSMI_STATUS_INVALID_ARGS;
  *bytes = mock_gpus[id].mem_used;
  return RSMI_STATUS_SUCCESS;
}

rsmi_status_t rsmi_dev_pci_throughput_get(uint32_t id, uint64_t *sent,
                                          uint64_t *recv, uint64_t *max_pkt) {
  if (id >= 2)
    return RSMI_STATUS_INVALID_ARGS;
  if (sent)
    *sent = mock_gpus[id].pci_sent;
  if (recv)
    *recv = mock_gpus[id].pci_recv;
  if (max_pkt)
    *max_pkt = mock_gpus[id].pci_max;
  return RSMI_STATUS_SUCCESS;
}
