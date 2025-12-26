#include "task.hpp"
#include "task_registry.hpp"
#include <cassert>
#include <iostream>

void testTaskCreation() {
    jts::Task t;
    t.name = "test_task";
    t.priority = 5;
    assert(t.name == "test_task");
    assert(t.priority == 5);
    assert(t.isValid());
    std::cout << "[PASS] Task creation\n";
}

void testRegistry() {
    jts::TaskRegistry reg;
    jts::Task t;
    t.name = "reg_test";
    t.priority = 7;
    
    uint64_t id = reg.registerTask(t);
    assert(id == 1);
    assert(reg.count() == 1);
    
    auto found = reg.getTask(id);
    assert(found.has_value());
    assert(found->name == "reg_test");
    
    reg.removeTask(id);
    assert(reg.count() == 0);
    std::cout << "[PASS] Registry\n";
}

int main() {
    std::cout << "=== Unit Tests ===\n";
    testTaskCreation();
    testRegistry();
    std::cout << "All tests passed!\n";
    return 0;
}