<div align="center">

<img src="https://capsule-render.vercel.app/api?type=waving&color=0,1a2744,2e4a7a&height=200&section=header&text=Sistema%20Legislativo&fontSize=52&fontColor=ffffff&fontAlignY=38&desc=Sistema%20de%20tramitacion%20legislativa%20en%20C&descAlignY=58&descSize=16" />

<br/>

![C](https://img.shields.io/badge/C-TurboC-1a2744?style=for-the-badge&logo=c&logoColor=white)
![Status](https://img.shields.io/badge/Estado-Completado-2e4a7a?style=flat-square&labelColor=1a1a1a&color=2e4a7a)
![Universidad](https://img.shields.io/badge/PUCV-INF--2223-3d3d3d?style=flat-square)

</div>

<br/>

<img src="https://capsule-render.vercel.app/api?type=rect&color=2e4a7a&height=2&section=header" />

### ▎DESCRIPCIÓN

**Sistema Legislativo** es un sistema de información de consola para simular la tramitación de proyectos de ley en Chile. Desarrollado en **C con TurboC**, aplicando las estructuras de datos vistas en cátedra: árboles binarios de búsqueda, listas enlazadas circulares y listas doblemente enlazadas.

El proyecto fue presentado y defendido oralmente ante el cuerpo docente como evaluación final del semestre.

<br/>

<img src="https://capsule-render.vercel.app/api?type=rect&color=2e4a7a&height=2&section=header" />

### ▎FUNCIONALIDADES

- **Árbol binario de proyectos de ley** — inserción, eliminación, búsqueda por ID y listado en orden.
- **Base de datos de políticos** — lista circular con operaciones CRUD completas.
- **Cámara de Diputados y Senadores** — listas doblemente enlazadas con agregar, eliminar, buscar y modificar.
- **Comisiones y votaciones** — gestión de comisiones de origen, revisora y mixta, con fases de votación.
- **Proceso de aprobación** — registro de decisión presidencial, publicación y control constitucional.
- **Funciones avanzadas** — edición del proceso legislativo completo de un proyecto específico.
- **Reportes** — conteo de diputados, senadores y listado de políticos involucrados en un proyecto.

<br/>

<img src="https://capsule-render.vercel.app/api?type=rect&color=2e4a7a&height=2&section=header" />

### ▎ESTRUCTURAS DE DATOS UTILIZADAS

| Estructura | Uso en el proyecto |
|---|---|
| ![](https://img.shields.io/badge/Arbol%20Binario%20de%20Busqueda-1a2744?style=flat-square) | Almacenamiento y búsqueda de proyectos de ley por ID |
| ![](https://img.shields.io/badge/Lista%20Circular%20Simple-2e4a7a?style=flat-square) | Base de datos de políticos |
| ![](https://img.shields.io/badge/Lista%20Doblemente%20Enlazada-6b7280?style=flat-square) | Cámara de Diputados y Cámara de Senadores |
| ![](https://img.shields.io/badge/Arreglos%20dinamicos-1a2744?style=flat-square) | Votos por votación y proyectos por político |
| ![](https://img.shields.io/badge/Structs%20anidados-2e4a7a?style=flat-square) | Comisiones, votaciones y proceso de aprobación |

<br/>

<img src="https://capsule-render.vercel.app/api?type=rect&color=2e4a7a&height=2&section=header" />

### ▎CAPTURAS

| Menu principal |
|---|
| ![Menu](screenshots/menu_principal.png) |

<br/>

<img src="https://capsule-render.vercel.app/api?type=rect&color=2e4a7a&height=2&section=header" />

### ▎ESTRUCTURA DEL PROYECTO

```
SistemaLegislativoC/
├── screenshots/
│   └── menu_principal.png
├── main.c          # Código fuente completo
└── README.md
```

<br/>

<img src="https://capsule-render.vercel.app/api?type=rect&color=2e4a7a&height=2&section=header" />

### ▎REQUISITOS Y EJECUCIÓN

**Opción 1 — TurboC (entorno original)**
```
Abrir main.c en TurboC  →  Compilar (Alt+F9)  →  Ejecutar (Ctrl+F9)
```

**Opción 2 — GCC en Windows (recomendado actualmente)**
```cmd
cd ruta\del\proyecto
C:\msys64\ucrt64\bin\gcc.exe main.c -o tramitacion.exe
tramitacion.exe
```

> Para instalar GCC en Windows se recomienda usar [MSYS2](https://www.msys2.org/) con el paquete `mingw-w64-ucrt-x86_64-gcc`.

<br/>

<img src="https://capsule-render.vercel.app/api?type=rect&color=2e4a7a&height=2&section=header" />

### ▎CONTEXTO ACADÉMICO

Proyecto desarrollado para **Estructura de Datos** INF2223 en la [Pontificia Universidad Católica de Valparaíso (PUCV)](https://www.pucv.cl), 3° semestre de Ingeniería en Informática durante el **2024**.

El sistema fue implementado como evaluación final integradora, aplicando todas las estructuras de datos vistas durante el semestre. Fue presentado y defendido oralmente ante el cuerpo docente y aprobado con una buena nota.

<br/>

<img src="https://capsule-render.vercel.app/api?type=rect&color=2e4a7a&height=2&section=header" />

### ▎AUTORES

<div align="center">

[![cord0990](https://img.shields.io/badge/@cord0990-6e1423?style=for-the-badge&logo=github&logoColor=white)](https://github.com/cord0990)
[![PatataSubnormal](https://img.shields.io/badge/@PatataSubnormal-e91e8c?style=for-the-badge&logo=github&logoColor=white)](https://github.com/PatataSubnormal)
[![cortadew](https://img.shields.io/badge/@cortadew-2e4a7a?style=for-the-badge&logo=github&logoColor=white)](https://github.com/cortadew)

</div>

<br/>

<img src="https://capsule-render.vercel.app/api?type=waving&color=0,1a2744,2e4a7a&height=100&section=footer" />