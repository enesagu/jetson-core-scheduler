#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "task.hpp"
#include "task_registry.hpp"
#include "scheduler.hpp"

namespace py = pybind11;

PYBIND11_MODULE(jetson_scheduler, m) {
    m.doc() = "Jetson Task Scheduler";

    py::enum_<jts::TaskType>(m, "TaskType")
        .value("CPU", jts::TaskType::CPU)
        .value("GPU", jts::TaskType::GPU)
        .value("IO", jts::TaskType::IO);

    py::class_<jts::Task>(m, "Task")
        .def(py::init<>())
        .def_readwrite("id", &jts::Task::id)
        .def_readwrite("name", &jts::Task::name)
        .def_readwrite("type", &jts::Task::type)
        .def_readwrite("priority", &jts::Task::priority)
        .def_readwrite("realtime", &jts::Task::realtime)
        .def("summary", &jts::Task::summary)
        .def("serialize", &jts::Task::serialize);

    py::class_<jts::TaskRegistry>(m, "TaskRegistry")
        .def(py::init<>())
        .def("register_task", &jts::TaskRegistry::registerTask)
        .def("list_tasks", &jts::TaskRegistry::listTasks)
        .def("count", &jts::TaskRegistry::count);

    py::class_<jts::Scheduler>(m, "Scheduler")
        .def(py::init<>())
        .def("add_task", &jts::Scheduler::addTask)
        .def("run_once", &jts::Scheduler::runOnce)
        .def("pending_count", &jts::Scheduler::pendingCount);
}