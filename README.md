# Microservices Issue Tracking System (ITS)

A high-performance, distributed **Issue Tracking System (ITS)** architected using a decoupled **Microservices Architecture** with the **Oat++ modern C++ web framework**. The system features a native, asynchronous Web-Client frontend that communicates across isolated service meshes via asynchronous RESTful APIs.

---

## 🏗️ System Architecture

This application splits core enterprise business logic across four distinct backend microservices running concurrently on independent network boundaries:

- **Issue Server (`Port 8301`)**: Manages core ticket lifecycles, lifecycle states, assignments, and workflow logic.
- **User Server (`Port 8302`)**: Handles account registries, permissions hierarchies, and comprehensive profile tracking (including `role` bindings and active group `workload` balancing).
- **Comment Server (`Port 8303`)**: Orchestrates the communication and collaborative thread engine, linking asynchronous dynamic user logs back to parenting tracking entries.
- **Search Server (`Port 8304`)**: The decoupled query and indexing engine running complex multi-variable filtering across the network.

---

## 🛠️ Tech Stack & Implementation Details

### Backend Core

- **Language:** C++11 / C++14
- **Framework:** [Oat++](https://oatpp.io/) (Zero-dependency, high-throughput HTTP framework)
- **API Documentation:** Built-in Swagger UI wrappers compiled natively into each server binary for direct interactive sandbox testing.

### Frontend Client

- **Interface:** Pure HTML5 / JavaScript (Vanilla ES6) utilizing asynchronous `Fetch API` networking.
- **UI System:** Modernized Bootstrap 5 responsive layout dashboard.

---

## 👨‍💻 Engineering Roles & Contributions

The project was developed using a **Backend-First lifecycle**, building and validating core schema structures and microservice interaction contracts before mapping responsive client interfaces.

### Arrol Cardoz

- **Microservice Engineering:** Designed and built the complete **Comment Server** (`commentServer`) backend engine using Oat++, structuring thread endpoints and dynamic response bodies.
- **Schema Extension:** Extended the base user tracking model to enforce granular property states, introducing explicit `name`, `email`, user `role` validation types, and operational `workload` capacity metrics.
- **Full-Stack Bridge:** Engineered the asynchronous **Comment Client** and integration scripts in JavaScript, establishing real-time query bindings to safely link frontend UI components with live remote server endpoints.

### Group Team Members

- **Claire Fleckney:** Developed the core **Issue Client** and accompanying lifecycle workflows.
- **Cyler Larocque:** Engineered the global **Search Client** logic and query integrations.
- **Josh Rist:** Designed the identity system and **User Client** management dashboards.

---

## 🚀 Environment Setup & Compilation

### 1. Prerequisites (Linux / WSL Environment)

Ensure your environment has a working C++ compiler and the standard build-essential packages installed:

```bash
sudo apt update && sudo apt install build-essential cmake
```

### 2. Compilation via Top-Level Makefile

The system utilizes a delegating Makefile system to automate microservice compilation across subdirectories. From the project root directory, execute:

```bash
make servers
```

### 3. Launching the Microservices

Because each server acts as an isolated microservice daemon, spawn each service executable in its own independent terminal window or background process thread:

```bash
./issueServer/issueServer       # Running on port 8301
./userServer/userServer         # Running on port 8302
./commentServer/commentServer   # Running on port 8303
./searchServer/searchServer     # Running on port 8304
```

### 4. Running the Web Client

Once all four microservices are listening on their designated ports, simply launch the client dashboard directly inside your web browser:

```text
Open file: /ITS-Client.html
```

---

## 📊 API Documentation & Sandbox Testing

Each independent microservice mounts an interactive documentation container out of the box. While the servers are running, navigate to any of the following endpoints to review endpoints, inspect JSON schemas, or fire sandbox network requests directly:

- **Issue Engine Docs:** `http://localhost:8301/swagger/ui`
- **User Engine Docs:** `http://localhost:8302/swagger/ui`
- **Comment Engine Docs:** `http://localhost:8303/swagger/ui`
- **Search Engine Docs:** `http://localhost:8304/swagger/ui`

---

## 🧹 Housekeeping

To wipe binary targets and reset build caches across all subprojects cleanly, invoke the top-level clean task:

```bash
make clean
```
