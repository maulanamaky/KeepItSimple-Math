# Scientific Calculator
---
This is Scientific Calculator using C++ as backend and HTML+CSS+JS as frontend.

<img width="670" height="760" alt="Image" src="https://github.com/user-attachments/assets/024e4eaf-42b2-4276-9f6f-72853d6ddb3e" />

Folder's Structure:
```
kism/
├── scientific.cpp     # C++ Server
├── httplib.h          
└── public/
    ├── kism.html
    ├── kism.css
    └── kism.js
```

How It Works?
<img width="511" height="81" alt="Image" src="https://github.com/user-attachments/assets/4f7387b1-bf39-4404-ab72-178bdb62b739" />

1. Button will be trigger javascript program if have clicked
2. Javascript fetch the C++ program logic on the server port 8080
3. Server give response JSON to javascript.

How to Run?

1. Download httplib.h
```
curl -LO https://github.com/yhirose/cpp-httplib/raw/refs/tags/latest/httplib.h
```

2. Compile the C++ program and run it
```
g++ scientific.cpp -o kism-server -pthread
./kism-server
```

3.  Then, you can open HTML on your browser

The KISM's Features:

| Endpoint    | Parameter    | Contoh               |
| ----------- | ------------ | -------------------- |
| `/add`      | `a`, `b`     | `/add?a=5&b=10`      |
| `/subtract` | `a`, `b`     | `/subtract?a=20&b=8` |
| `/multiply` | `a`, `b`     | `/multiply?a=6&b=7`  |
| `/divide`   | `a`, `b`     | `/divide?a=9&b=2`    |
| `/modulo`   | `a`, `b`     | `/modulo?a=10&b=3`   |
| `/sin`      | `a` (degree) | `/sin?a=30`          |
| `/cos`      | `a` (degree) | `/cos?a=30`          |
| `/cot`      | `a` (degree) | `/cot?a=30`          |
| `/log`      | `a`          | `/log?a=100`         |
| `/square`   | `a`          | `/square?a=9`        |
| `/cube`     | `a`          | `/cube?a=3`          |

> Next Development:
> - It can be add more operation scientific math
> - Subscription feature (joke)
