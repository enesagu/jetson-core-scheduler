#!/usr/bin/env python3
"""Jetson Task Scheduler - Python Demo"""

import sys
sys.path.insert(0, '../build')

import jetson_scheduler as js

def main():
    print("=== Jetson Task Scheduler Python Demo ===\n")
    
    scheduler = js.Scheduler()
    
    # Task 1: Kamera
    t1 = js.Task()
    t1.name = "camera_capture"
    t1.type = js.TaskType.CPU
    t1.priority = 7
    scheduler.add_task(t1)
    
    # Task 2: AI
    t2 = js.Task()
    t2.name = "neural_inference"
    t2.type = js.TaskType.GPU
    t2.priority = 10
    t2.realtime = True
    scheduler.add_task(t2)
    
    # Task 3: Logger
    t3 = js.Task()
    t3.name = "log_writer"
    t3.type = js.TaskType.IO
    t3.priority = 2
    scheduler.add_task(t3)
    
    print(f"Bekleyen task: {scheduler.pending_count()}")
    
    # Çalıştır
    while scheduler.run_once():
        pass
    
    print("\nDemo tamamlandı!")

if __name__ == "__main__":
    main()