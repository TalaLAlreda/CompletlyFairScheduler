#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

struct Process
{
    int pid;
    int burstTime;
    int vruntime;
    int arrivalTime;

    Process(int p, int bt, int at)
        : pid(p), burstTime(bt), vruntime(0), arrivalTime(at) {}
};

struct compareProcess
{
    bool operator()(const Process *p1, const Process *p2)
    {
        return p1->vruntime > p2->vruntime;
    }
};

class CompletelyFairScheduler
{
private:
    priority_queue<Process *, vector<Process *>, compareProcess> runQueue;
    int currentTime;

public:
    CompletelyFairScheduler() : currentTime(0) {}

    void addProcess(Process *proc)
    {
        runQueue.push(proc);
    }

    void run()
    {
        while (!runQueue.empty())
        {
            Process *proc = runQueue.top();
            runQueue.pop();

            int runtime = min(proc->burstTime, 10); // Simulate a time slice of 10 units

            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            string cmd = "cmd /c timeout /t " + to_string(runtime) + " /nobreak > nul";
            if (CreateProcess(NULL, const_cast<LPSTR>(cmd.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
            {
                cout << "Running process " << proc->pid << " for " << runtime << " units\n";
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            else
            {
                cerr << "CreateProcess failed!\n";
                exit(1);
            }

            proc->burstTime -= runtime;
            currentTime += runtime;
            proc->vruntime += runtime;

            if (proc->burstTime > 0)
            {
                runQueue.push(proc);
            }
        }
    }
};

int main()
{
    CompletelyFairScheduler scheduler;

    // Adding processes to the scheduler
    scheduler.addProcess(new Process(1, 30, 0));
    scheduler.addProcess(new Process(2, 20, 0));
    scheduler.addProcess(new Process(3, 10, 0));

    scheduler.run();

    return 0;
}
