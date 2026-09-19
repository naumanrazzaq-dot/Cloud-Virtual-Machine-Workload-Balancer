#include <iostream>
#include <string>
using namespace std;

class CloudNode {
protected:
    string nodeID;
    int assignedCores;

public:
    static int totalAllocatedRAM;

    CloudNode(string nodeID, int assignedCores) {
        this->nodeID = nodeID;
        this->assignedCores = assignedCores;
    }

    virtual ~CloudNode() {
        cout << "[TERMINATED] Node " << nodeID << " decommissioned from cluster." << endl;
    }

    virtual double calculateMaxIOPS() = 0;
    virtual void inspectNode() = 0;
};

int CloudNode::totalAllocatedRAM = 0;

class ComputeOptimizedNode : public CloudNode {
private:
    int turboClockSpeedGHz;
    const int reservedRAM = 16;

public:
    ComputeOptimizedNode(string nodeID, int assignedCores, int turboClockSpeedGHz)
        : CloudNode(nodeID, assignedCores), turboClockSpeedGHz(turboClockSpeedGHz) {
        totalAllocatedRAM += reservedRAM;
    }

    ~ComputeOptimizedNode() override {
        totalAllocatedRAM -= reservedRAM;
    }

    double calculateMaxIOPS() override {
        return (assignedCores * 1500.0) + (turboClockSpeedGHz * 400.0);
    }

    void inspectNode() override {
        cout << "\n--- [COMPUTE-OPTIMIZED NODE] ---" << endl;
        cout << "Node ID         : " << nodeID << endl;
        cout << "CPU Cores       : " << assignedCores << endl;
        cout << "Clock Speed     : " << turboClockSpeedGHz << " GHz" << endl;
        cout << "Reserved RAM    : " << reservedRAM << " GB" << endl;
        cout << "Max IOPS        : " << calculateMaxIOPS() << endl;
    }
};

class MemoryOptimizedNode : public CloudNode {
private:
    int attachedNVMeDrives;
    const int reservedRAM = 64;

public:
    MemoryOptimizedNode(string nodeID, int assignedCores, int attachedNVMeDrives)
        : CloudNode(nodeID, assignedCores), attachedNVMeDrives(attachedNVMeDrives) {
        totalAllocatedRAM += reservedRAM;
    }

    ~MemoryOptimizedNode() override {
        totalAllocatedRAM -= reservedRAM;
    }

    double calculateMaxIOPS() override {
        return (assignedCores * 800.0) + (attachedNVMeDrives * 5000.0);
    }

    void inspectNode() override {
        cout << "\n--- [MEMORY-OPTIMIZED NODE] ---" << endl;
        cout << "Node ID         : " << nodeID << endl;
        cout << "CPU Cores       : " << assignedCores << endl;
        cout << "NVMe Drives     : " << attachedNVMeDrives << endl;
        cout << "Reserved RAM    : " << reservedRAM << " GB" << endl;
        cout << "Max IOPS        : " << calculateMaxIOPS() << endl;
    }
};

int main() {
    CloudNode* cluster[2];

    cluster[0] = new ComputeOptimizedNode("C5-NODE-01", 8, 4);
    cluster[1] = new MemoryOptimizedNode("R5-NODE-02", 16, 2);

    for (int i = 0; i < 2; i++) {
        cluster[i]->inspectNode();
    }

    cout << "\n=============================================" << endl;
    cout << "Total Active Cluster RAM: " << CloudNode::totalAllocatedRAM << " GB" << endl;
    cout << "=============================================\n" << endl;

    for (int i = 0; i < 2; i++) {
        delete cluster[i];
        cluster[i] = nullptr;
    }

    cout << "\n=============================================" << endl;
    cout << "Total Active Cluster RAM After Teardown: " << CloudNode::totalAllocatedRAM << " GB" << endl;
    cout << "=============================================" << endl;

    return 0;
}