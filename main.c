#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructuras para politicos
struct politico {
    char *nombre;
    char *cargo;
    char *Area;
    char *rut;
    struct datosProyectoLey **ProyectosInvolucrados;
    int plibreLeyes;
};

struct nodopolitico {
    struct politico *datos;
    struct nodopolitico *sig, *ant;
};

struct datosProyectoLey {
    int id;
    int VigenciaProceso;
    char *fechaEntrada;
    char *propuesta;
    char *tipoPropuesta;
    int estado;

    struct comision *CamaraDeOrigen;
    struct votaciones **VotacionLey[3];
    struct comision *CamaraRevisoria;
    struct ProcesoDeAprobacion *ProcesoAprobatorio;
    struct comision *ComisionMixta;
};

struct nodoProyectoLey {
    struct datosProyectoLey *propuesta;
    struct nodoProyectoLey *izq, *der;
};

struct datosVoto {
    int tipo;
    struct politico *votante;
};

struct votaciones {
    int FaseVotacion;
    struct datosVoto **votos;
    int PlibreVotaciones;
    struct datosProyectoLey *propuesta;
};

struct comision {
    struct politico **parlamentarios;
    int PlibrePoliticos;
    struct datosProyectoLey *propuesta;
    char *informe;
    char *Urgencia;
    char *nombreComision;
};

struct ProcesoDeAprobacion {
    char *nombrePresidente;
    char *fecha;
    int Publicacion;
    char* TipoPublicacion;
    int ControlConstitucional;
};

struct TramitacionLegislativa {
    struct nodoProyectoLey *ArboldeProyectos;
    struct nodopolitico *BDPoliticos;
    struct nodopolitico *CamaraDeDiputados;
    struct nodopolitico *CamaraDeSenadores;
};

//Funciones Arbol de Proyectos
struct nodoProyectoLey* CrearNodoProyecto(int id, int vigencia, const char* fecha, const char* descripcion, const char* tipo, int estado)
{
    struct nodoProyectoLey* nuevoNodo = (struct nodoProyectoLey*)malloc(sizeof(struct nodoProyectoLey));
    nuevoNodo->propuesta = (struct datosProyectoLey*)malloc(sizeof(struct datosProyectoLey));

    nuevoNodo->propuesta->id = id;
    nuevoNodo->propuesta->VigenciaProceso = vigencia;
    nuevoNodo->propuesta->fechaEntrada = strdup(fecha);
    nuevoNodo->propuesta->propuesta = strdup(descripcion);
    nuevoNodo->propuesta->tipoPropuesta = strdup(tipo);
    nuevoNodo->propuesta->estado = estado;

    nuevoNodo->propuesta->CamaraRevisoria = NULL;
    nuevoNodo->propuesta->CamaraDeOrigen = NULL;
    nuevoNodo->propuesta->ProcesoAprobatorio = NULL;

    nuevoNodo->izq = nuevoNodo->der = NULL;
    return nuevoNodo;
}

struct nodoProyectoLey* InsertarProyecto(struct nodoProyectoLey* raiz, struct nodoProyectoLey* nuevoNodo) {
    if (raiz == NULL) {
        return nuevoNodo;
    }
    if (nuevoNodo->propuesta->id < raiz->propuesta->id) {
        raiz->izq = InsertarProyecto(raiz->izq, nuevoNodo);
    } else {
        raiz->der = InsertarProyecto(raiz->der, nuevoNodo);
    }
    return raiz;
}

struct datosProyectoLey* BuscarProyectoEnArbol(struct nodoProyectoLey* raiz, int id) {
    if (raiz == NULL) return NULL;
    if (id == raiz->propuesta->id) return raiz->propuesta;
    else if (id < raiz->propuesta->id) return BuscarProyectoEnArbol(raiz->izq, id);
    else return BuscarProyectoEnArbol(raiz->der, id);
}

void BuscarYMostrarProyecto(struct nodoProyectoLey* arbol, int id)
{
    struct datosProyectoLey* proyecto = BuscarProyectoEnArbol(arbol, id);
    if (proyecto) {
        printf("Proyecto encontrado: ID=%d, Descripcion=%s\n", proyecto->id, proyecto->propuesta);
    } else {
        printf("Proyecto no encontrado.\n");
    }
}

struct nodoProyectoLey* EliminarProyecto(struct nodoProyectoLey* raiz, int id_buscado)
{
    struct nodoProyectoLey* temp;
    if (raiz == NULL) return NULL;

    if (id_buscado < (raiz->propuesta->id)) {
        raiz->izq = EliminarProyecto(raiz->izq, id_buscado);
    } else if (id_buscado > raiz->propuesta->id) {
        raiz->der = EliminarProyecto(raiz->der, id_buscado);
    } else {
        if (raiz->izq == NULL) {
            temp = raiz->der;
            return temp;
        } else if (raiz->der == NULL) {
            temp = raiz->izq;
            return temp;
        }
        temp = raiz->der;
        while (temp->izq != NULL) {
            temp = temp->izq;
        }
        raiz->propuesta = temp->propuesta;
        raiz->der = EliminarProyecto(raiz->der, temp->propuesta->id);
    }
    return raiz;
}

void ListarProyectos(struct nodoProyectoLey* raiz) {
    if (raiz == NULL) return;
    ListarProyectos(raiz->izq);
    printf("ID: %d, Vigencia: %d, Fecha: %s, Descripcion: %s, Tipo: %s, Estado: %d\n",
           raiz->propuesta->id,
           raiz->propuesta->VigenciaProceso,
           raiz->propuesta->fechaEntrada,
           raiz->propuesta->propuesta,
           raiz->propuesta->tipoPropuesta,
           raiz->propuesta->estado);
    ListarProyectos(raiz->der);
}

void ModificarProyecto(struct nodoProyectoLey* raiz, int id) {
    struct datosProyectoLey* proyecto = BuscarProyectoEnArbol(raiz, id);
    int nuevaVigencia, nuevoEstado;
    char nuevaFecha[10], nuevaDescripcion[100], nuevoTipo[50];

    if (proyecto == NULL) {
        printf("Proyecto con ID %d no encontrado.\n", id);
        return;
    }

    printf("Proyecto encontrado. Ingrese los nuevos datos:\n");
    printf("Ingrese nueva vigencia (1=en tramitacion, 0=fuera de proceso): ");
    scanf("%d", &nuevaVigencia);
    printf("Ingrese nueva fecha de entrada (dd-mm-aaaa): ");
    scanf("%s", nuevaFecha);
    printf("Ingrese nueva descripcion de la propuesta: ");
    scanf("%s", nuevaDescripcion);
    printf("Ingrese nuevo tipo de propuesta: ");
    scanf("%s", nuevoTipo);
    printf("Ingrese nuevo estado (1=aprobada, -1=modificada, etc.): ");
    scanf("%d", &nuevoEstado);

    proyecto->VigenciaProceso = nuevaVigencia;
    strncpy(proyecto->fechaEntrada, nuevaFecha, sizeof(nuevaFecha));
    strncpy(proyecto->propuesta, nuevaDescripcion, sizeof(nuevaDescripcion));
    strncpy(proyecto->tipoPropuesta, nuevoTipo, sizeof(nuevoTipo));
    proyecto->estado = nuevoEstado;

    printf("Proyecto modificado con exito.\n");
}

void MenuArbolDeProyectos(struct TramitacionLegislativa* tl) {
    struct nodoProyectoLey* nuevoNodo;
    char opt;
    short estado, vigencia;
    char fecha[10], descripcion[100], tipo[50];
    int id;

    do {
        printf("\n|------------------------------------|\n");
        printf("|  Bienvenido al arbol de Proyectos  |\n");
        printf("|-------Seleccione una opcion--------|\n");
        printf("| 1. Agregar                         |\n");
        printf("| 2. Eliminar                        |\n");
        printf("| 3. Buscar                          |\n");
        printf("| 4. Modificar                       |\n");
        printf("| 5. Listar                          |\n");
        printf("| 6. Volver al Menu Principal        |\n");
        printf("|------------------------------------|\n");
        printf("Seleccione una opcion: ");
        scanf(" %c", &opt);

        switch (opt) {
            case '1':
                printf("Ingrese ID del proyecto: ");
                scanf("%d", &id);
                printf("Ingrese vigencia (1=en tramitacion, 0=fuera de proceso): ");
                scanf("%d", &vigencia);
                printf("Ingrese fecha de entrada (dd-mm-aaaa): ");
                scanf("%s", fecha);
                printf("Ingrese descripcion de la propuesta: ");
                scanf("%s", descripcion);
                printf("Ingrese tipo de propuesta: ");
                scanf("%s", tipo);
                printf("Ingrese estado del proyecto (1=aprobada, -1=modificada, etc.): ");
                scanf("%d", &estado);
                nuevoNodo = CrearNodoProyecto(id, vigencia, fecha, descripcion, tipo, estado);
                tl->ArboldeProyectos = InsertarProyecto(tl->ArboldeProyectos, nuevoNodo);
                printf("Proyecto agregado con exito.\n");
                break;
            case '2':
                printf("Ingrese el ID del proyecto a eliminar: ");
                scanf("%d", &id);
                tl->ArboldeProyectos = EliminarProyecto(tl->ArboldeProyectos, id);
                printf("Proyecto eliminado.\n");
                break;
            case '3':
                printf("Ingrese el ID del proyecto a buscar: ");
                scanf("%d", &id);
                BuscarYMostrarProyecto(tl->ArboldeProyectos, id);
                break;
            case '4':
                printf("Ingrese el ID del proyecto a modificar: ");
                scanf("%d", &id);
                ModificarProyecto(tl->ArboldeProyectos, id);
                break;
            case '5':
                printf("Lista de todos los proyectos:\n");
                ListarProyectos(tl->ArboldeProyectos);
                break;
            case '6':
                printf("Volviendo al Menu Principal...\n\n");
                break;
            default:
                printf("Opcion invalida, intente nuevamente.\n");
                break;
        }
    } while (opt != '6');
}

struct nodopolitico* CrearNodoPolitico(const char *nombre, char *cargo, const char *area, const char *rut, int plibreLeyes)
{
    struct nodopolitico* nuevoNodo = (struct nodopolitico*)malloc(sizeof(struct nodopolitico));
    nuevoNodo->datos = (struct politico*)malloc(sizeof(struct politico));

    nuevoNodo->datos->nombre = strdup(nombre);
    nuevoNodo->datos->cargo = strdup(cargo);
    nuevoNodo->datos->Area = strdup(area);
    nuevoNodo->datos->rut = strdup(rut);
    nuevoNodo->datos->plibreLeyes = plibreLeyes;
    nuevoNodo->datos->ProyectosInvolucrados = NULL;

    nuevoNodo->sig = NULL;
    nuevoNodo->ant = NULL;
    return nuevoNodo;
}

struct nodopolitico* InsertarPolitico(struct nodopolitico* head, struct nodopolitico* nuevoNodo)
{
    struct nodopolitico* actual;
    if (head == NULL) {
        nuevoNodo->sig = nuevoNodo;
        return nuevoNodo;
    }
    actual = head;
    while (actual->sig != head) {
        actual = actual->sig;
    }
    actual->sig = nuevoNodo;
    nuevoNodo->sig = head;
    return head;
}

struct politico* BuscarPolitico(struct nodopolitico* head, const char* rut)
{
    struct nodopolitico* actual;
    if (head == NULL) {
        printf("La lista esta vacia.\n");
        return NULL;
    }
    actual = head;
    do {
        if (strcmp(actual->datos->rut, rut) == 0) {
            printf("Politico encontrado: Nombre=%s, Cargo=%s, Area=%s, RUT=%s, Proyectos libres=%d\n",
                   actual->datos->nombre,
                   actual->datos->cargo,
                   actual->datos->Area,
                   actual->datos->rut,
                   actual->datos->plibreLeyes);
            return actual->datos;
        }
        actual = actual->sig;
    } while (actual != head);

    printf("Politico con RUT %s no encontrado.\n", rut);
    return NULL;
}

struct nodopolitico* EliminarPolitico(struct nodopolitico* head, const char* rut_buscado)
{
    struct nodopolitico *actual, *anterior;
    if (head == NULL) return NULL;

    actual = head;
    anterior = NULL;

    if (actual->sig == head && strcmp(actual->datos->rut, rut_buscado) == 0) {
        return NULL;
    }

    do {
        if (strcmp(actual->datos->rut, rut_buscado) == 0) {
            if (anterior == NULL) {
                struct nodopolitico* ultimo = head;
                while (ultimo->sig != head) {
                    ultimo = ultimo->sig;
                }
                head = actual->sig;
                ultimo->sig = head;
            } else {
                anterior->sig = actual->sig;
            }
            printf("Politico con RUT %s eliminado de la lista.\n", rut_buscado);
            return head;
        }
        anterior = actual;
        actual = actual->sig;
    } while (actual != head);

    printf("Politico con RUT %s no encontrado.\n", rut_buscado);
    return head;
}

void ListarPoliticos(struct nodopolitico* head) {
    struct nodopolitico* actual;
    if (head == NULL) {
        printf("No hay politicos en la lista.\n");
        return;
    }
    actual = head;
    do {
        printf("Nombre: %s, Cargo: %s, Area: %s, RUT: %s, Proyectos libres: %d\n",
               actual->datos->nombre,
               actual->datos->cargo,
               actual->datos->Area,
               actual->datos->rut,
               actual->datos->plibreLeyes);
        actual = actual->sig;
    } while (actual != head);
}

void ModificarPolitico(struct nodopolitico* head, const char* rut) {
    struct politico* politico_actual = BuscarPolitico(head, rut);
    short nuevosProyectosLibres;
    char nuevoNombre[100], nuevaArea[50], nuevoCargo[50];
    if (politico_actual == NULL) {
        printf("Politico con RUT %s no encontrado.\n", rut);
        return;
    }

    printf("Politico encontrado. Ingrese los nuevos datos:\n");
    printf("Ingrese nuevo nombre: ");
    scanf("%s", nuevoNombre);
    printf("Ingrese nuevo cargo (1=Senador, 2=Diputado): ");
    scanf("%s", &nuevoCargo);
    printf("Ingrese nueva area: ");
    scanf("%s", nuevaArea);
    printf("Ingrese la cantidad de proyectos libres: ");
    scanf("%hd", &nuevosProyectosLibres);

    politico_actual->nombre = strdup(nuevoNombre);
    politico_actual->cargo = nuevoCargo;
    politico_actual->Area = strdup(nuevaArea);
    politico_actual->plibreLeyes = nuevosProyectosLibres;

    printf("Datos del politico modificados con exito.\n");
}

void MenuBasededatosPolitiicos(struct TramitacionLegislativa* tl) {
    struct politico* politico_encontrado;
    struct nodopolitico* nuevoNodo;
    char opt;
    char nombre[100], area[30], rut[12], cargo[40];
    int plibreLeyes;

    do {
        printf("\n|------------------------------------|\n");
        printf("|    Gestion de Base de Politicos    |\n");
        printf("|-------Seleccione una opcion--------|\n");
        printf("| 1. Agregar                         |\n");
        printf("| 2. Eliminar                        |\n");
        printf("| 3. Buscar                          |\n");
        printf("| 4. Modificar                       |\n");
        printf("| 5. Listar                          |\n");
        printf("| 6. Volver al Menu Principal        |\n");
        printf("|------------------------------------|\n");
        printf("Seleccione una opcion: ");
        scanf(" %c", &opt);

        switch (opt) {
            case '1':
                printf("Ingrese nombre del politico: ");
                scanf("%s", nombre);
                printf("Ingrese cargo del politico: ");
                scanf("%s", cargo);
                printf("Ingrese area de especializacion del politico: ");
                scanf("%s", area);
                printf("Ingrese RUT del politico: ");
                scanf("%s", rut);
                printf("Ingrese la cantidad de proyectos libres del politico: ");
                scanf("%d", &plibreLeyes);
                nuevoNodo = CrearNodoPolitico(nombre, cargo, area, rut, plibreLeyes);
                tl->BDPoliticos = InsertarPolitico(tl->BDPoliticos, nuevoNodo);
                printf("Politico agregado con exito.\n");
                break;
            case '2':
                printf("Ingrese el RUT del politico a eliminar: ");
                scanf("%s", rut);
                tl->BDPoliticos = EliminarPolitico(tl->BDPoliticos, rut);
                printf("Politico eliminado.\n");
                break;
            case '3':
                printf("Ingrese el RUT del politico a buscar: ");
                scanf("%s", rut);
                politico_encontrado = BuscarPolitico(tl->BDPoliticos, rut);
                if (politico_encontrado) {
                    printf("Politico encontrado: Nombre=%s, Cargo=%s, Area=%s, RUT=%s, Proyectos libres=%d\n",
                            politico_encontrado->nombre,
                            politico_encontrado->cargo,
                            politico_encontrado->Area,
                            politico_encontrado->rut,
                            politico_encontrado->plibreLeyes);
                } else {
                    printf("Politico no encontrado.\n");
                }
                break;
            case '4':
                printf("Ingrese el RUT del politico a modificar: ");
                scanf("%s", rut);
                ModificarPolitico(tl->BDPoliticos, rut);
                break;
            case '5':
                printf("Lista de todos los politicos:\n");
                ListarPoliticos(tl->BDPoliticos);
                break;
            case '6':
                printf("Volviendo al Menu Principal...\n\n");
                break;
            default:
                printf("Opcion invalida, intente nuevamente.\n");
                break;
        }
    } while (opt != '6');
}

char BuscarRutPoliticos(struct nodopolitico *BDPolitico, const char *Rut)
{
    struct nodopolitico *rec;
    if (BDPolitico == NULL) return '0';
    rec = BDPolitico;
    do {
        if (strcmp(rec->datos->rut, Rut) == 0) return '1';
        rec = rec->sig;
    } while (rec != BDPolitico);
    return '0';
}

struct politico * RegresarInformacionDeCamara(struct nodopolitico *Camara, const char *Rut)
{
    struct nodopolitico *rec;
    if (Camara == NULL) return NULL;
    rec = Camara;
    while (rec != NULL) {
        if (strcmp(rec->datos->rut, Rut) == 0) return rec->datos;
        rec = rec->sig;
    }
    return NULL;
}

struct politico *TraspasarPoliticoCamara(struct nodopolitico *BDPolitico, const char *Rut)
{
    struct nodopolitico *rec;
    if (BDPolitico == NULL) return NULL;
    rec = BDPolitico;
    do {
        if (strcmp(rec->datos->rut, Rut) == 0) return (rec->datos);
        rec = rec->sig;
    } while (rec != BDPolitico);
    return NULL;
}

void AgregarACamara(struct nodopolitico **Camara, struct TramitacionLegislativa *TL) {
    char RutBuscado[20];
    struct nodopolitico *Nuevo, *rec;
    Nuevo = NULL;
    if (TL->BDPoliticos == NULL) {
        printf("|--Error: La base de datos de politicos esta vacia.--|\n");
    } else {
        printf("|--Ingrese rut del parlamentario a agregar sin puntos ni guion--|\nRut:\n");
        scanf("%s", &RutBuscado);
        if (BuscarRutPoliticos(TL->BDPoliticos, RutBuscado) == '0') {
            printf("|--Rut no encontrado en la base de datos--|\n");
        } else {
            Nuevo = (struct nodopolitico*)malloc(sizeof(struct nodopolitico));
            if ((*Camara) == NULL) {
                Nuevo->datos = (TraspasarPoliticoCamara(TL->BDPoliticos, RutBuscado));
                Nuevo->ant = NULL;
                Nuevo->sig = NULL;
                *Camara = Nuevo;
            } else {
                rec = *Camara;
                while(rec->sig != NULL) rec = rec->sig;
                Nuevo->datos = (TraspasarPoliticoCamara(TL->BDPoliticos, RutBuscado));
                rec->sig = Nuevo;
                Nuevo->ant = rec;
                Nuevo->sig = NULL;
            }
            printf("|--Politico agregado correctamente--|\n");
        }
    }
}

void EliminarDeCamara(struct nodopolitico **Camara) {
    char RutBuscado[20];
    struct nodopolitico *rec;

    if ((*Camara) == NULL) {
        printf("|--No se puede eliminar elementos, la lista esta vacia--|\n");
    } else {
        printf("|--Ingrese rut del parlamentario que desea eliminar--|\nRut:\n");
        scanf("%s", &RutBuscado);
        if (strcmp((*Camara)->datos->rut, RutBuscado) == 0) {
            (*Camara) = (*Camara)->sig;
            (*Camara)->ant = NULL;
            printf("|--Se realizo con exito la eliminacion--|\n");
        } else {
            rec = *Camara;
            while (rec != NULL) {
                if (strcmp(rec->datos->rut, RutBuscado) == 0) {
                    if (rec->sig == NULL) rec->ant->sig = NULL;
                    else {
                        rec->sig->ant = rec->ant;
                        rec->ant->sig = rec->sig;
                    }
                    break;
                }
                rec = rec->sig;
            }
            printf("|--Se realizo con exito la eliminacion--|\n");
        }
    }
}

void MostrarProyectosInvolucrados(struct datosProyectoLey **Leyes, int Plibre) {
    int i;
    printf("|---Id de Proyectos de ley involucrado---|\n");
    for (i = 0; i < Plibre; i++) {
        printf("| -Ley %d\n", Leyes[i]->id);
    }
    printf("|---------------Termino-----------------|\n");
}

void BuscarEnCamara(struct nodopolitico *Camara) {
    struct nodopolitico *rec = Camara;
    char RutBuscado[20];
    int Encontrado = 0;

    if (Camara == NULL) {
        printf("|--No se pudo realizar la operacion, la lista esta vacia--|\n");
    } else {
        printf("|--Ingrese Rut del parlamentario a buscar.--|\nRut:\n");
        scanf("%s", &RutBuscado);
        while(rec != NULL) {
            if (strcmp(rec->datos->rut, RutBuscado) == 0) {
                printf("|---------Politico encontrado---------|\n");
                printf("|--Mostrando informacion de Parlamentario--|\n");
                printf("|Nombre: %s\n", rec->datos->nombre);
                printf("|Area: %s\n", rec->datos->Area);
                printf("|Cargo: %s\n", rec->datos->cargo);
                printf("|Rut: %s\n", rec->datos->rut);
                MostrarProyectosInvolucrados(rec->datos->ProyectosInvolucrados, rec->datos->plibreLeyes);
                Encontrado = 1;
                break;
            }
            rec = rec->sig;
        }
        if (Encontrado == 0) printf("|--------Politico no fue encontrado-------|\n");
    }
}

void modificarString(char *str) {
    char temp[1000];
    printf("Ingrese una nueva cadena: ");
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        printf("Error al leer la entrada.\n");
        return;
    }
    temp[strcspn(temp, "\n")] = '\0';
    str = malloc(strlen(temp) + 1);
    if (str == NULL) { printf("Error al asignar memoria.\n"); exit(1); }
    strcpy(str, temp);
}

void ModificarPoliticos(char **str) {
    char temp[1000];
    printf("Ingrese una nueva cadena: ");
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        printf("Error al leer la entrada.\n");
        return;
    }
    temp[strcspn(temp, "\n")] = '\0';
    *str = malloc(strlen(temp) + 1);
    if (*str == NULL) { printf("Error al asignar memoria.\n"); exit(1); }
    strcpy(*str, temp);
}

void ModificarListaEnPoliticos(struct datosProyectoLey **ProyectosInvolucrados, int *Plibre) {
    int IdLey, i, encontrado;
    encontrado = 0;
    printf("|---Escriba el ID de ley que desea modificar---|\n");
    scanf("%d", &IdLey);
    for(i = 0; i < (int)Plibre; i++) {
        if (ProyectosInvolucrados[i]->id == IdLey) {
            encontrado = 1;
            printf("|---Se encontro el proyecto a modificar---|\n");
            break;
        }
    }
    if (encontrado == 0) printf("|---No se encontro el proyecto de ley a modificar---|\n");
}

void MenuModificar(struct politico *Camara) {
    char opt;
    do {
        printf("|---Escriba el area que desea editar---|\n");
        printf("| 1. Nombre                            |\n");
        printf("| 2. Cargo                             |\n");
        printf("| 3. Area                              |\n");
        printf("| 4. Rut                               |\n");
        printf("| 5. Lista de Proyectos participantes  |\n");
        printf("| 6. Volver                            |\n");
        scanf(" %c", &opt);

        switch (opt) {
            case '1': ModificarPoliticos(&(Camara->nombre)); break;
            case '2': ModificarPoliticos(&(Camara->cargo)); break;
            case '3': ModificarPoliticos(&(Camara->Area)); break;
            case '4': ModificarPoliticos(&(Camara->rut)); break;
            case '5': ModificarListaEnPoliticos(Camara->ProyectosInvolucrados, &(Camara->plibreLeyes)); break;
            case '6': printf("Volviendo al Menu...\n"); break;
            default: printf("Opcion invalida, intente nuevamente.\n"); break;
        }
    } while (opt != '6');
}

void BuscarModificadoEnCamara(struct nodopolitico *Camara) {
    char RutBuscado[20];
    printf("|---Ingrese rut del parlamentario a modificar---|\nRut: ");
    scanf("%s", &RutBuscado);
    if (RegresarInformacionDeCamara(Camara, RutBuscado) == NULL) {
        printf("|------Rut no encontrado en la lista de parlamentarios------|\n");
    } else {
        printf("|--------Rut encontrado en la lista de parlamentarios-------|\n");
        MenuModificar(RegresarInformacionDeCamara(Camara, RutBuscado));
    }
}

void ListarCamara(struct nodopolitico *Camara) {
    int cont;
    struct nodopolitico *rec;
    cont = 1;
    rec = Camara;
    if (Camara == NULL) {
        printf("|---No se pudo listar, la lista se encuentra vacia---|\n");
    } else {
        while(rec != NULL) {
            printf("|%d. %s\n", cont, rec->datos->nombre);
            cont++;
            rec = rec->sig;
        }
        printf("|--Se han mostrado todos los parlamentarios en la camara--|\n");
        printf("|--Total: %d--|\n", cont - 1);
    }
}

void MenuCamara(struct TramitacionLegislativa *TL, struct nodopolitico *Camara) {
    char opt;
    do {
        printf("\n|-------------------------------------|\n");
        printf("|   Bienvenido a la Camara            |\n");
        printf("|-------Seleccione una opcion---------|\n");
        printf("| 1. Agregar                          |\n");
        printf("| 2. Eliminar                         |\n");
        printf("| 3. Buscar                           |\n");
        printf("| 4. Modificar                        |\n");
        printf("| 5. Listar                           |\n");
        printf("| 6. Volver al Menu Principal         |\n");
        printf("|-------------------------------------|\n");
        printf("Seleccione una opcion: ");
        scanf(" %c", &opt);

        switch (opt) {
            case '1': AgregarACamara(&Camara, TL); break;
            case '2': EliminarDeCamara(&Camara); break;
            case '3': BuscarEnCamara(Camara); break;
            case '4': BuscarModificadoEnCamara(Camara); break;
            case '5': ListarCamara(Camara); break;
            case '6': printf("Volviendo al Menu Principal...\n"); break;
            default: printf("Opcion invalida, intente nuevamente.\n"); break;
        }
    } while (opt != '6');
}

void EditarParlamentarios(struct politico **Parlamentarios, int plibre) {
    int i;
    char opt;
    if(*Parlamentarios == NULL) {
        printf("|---La lista esta vacia, dando memoria a la lista---|\n");
        (*Parlamentarios) = (struct politico *)malloc(sizeof(struct politico));
    }
    printf("|---La lista contiene %d Elementos---|\n", plibre);
    printf("|---Escriba la posicion del elemento que desea editar---|\n");
    scanf("%d", &i);
    printf("|---Mostrando datos de la posicion %d---|\n", i);
    printf("| 1-Nombre: %s\n", Parlamentarios[i]->nombre);
    printf("| 2-Cargo: %s\n", Parlamentarios[i]->cargo);
    printf("| 3-Area: %s\n", Parlamentarios[i]->Area);
    printf("| 4-Rut: %s\n", Parlamentarios[i]->rut);
    printf("|---Escriba que desea editar---|\n");
    scanf(" %c", &opt);
    switch (opt) {
        case '1': modificarString(Parlamentarios[i]->nombre); break;
        case '2': modificarString(Parlamentarios[i]->cargo); break;
        case '3': modificarString(Parlamentarios[i]->Area); break;
        case '4': modificarString(Parlamentarios[i]->rut); break;
        case '0': printf("Saliendo del Menu...\n"); break;
        default: printf("Opcion invalida, intente nuevamente.\n"); break;
    }
}

void ModificarComision(struct comision **Comision) {
    char opt;
    if(*Comision == NULL) (*Comision) = malloc(sizeof(struct comision));

    do {
        printf("|---Que area desea modificar?---|\n");
        printf("| 1. Lista de Parlamentarios    |\n");
        printf("| 2. Informe                    |\n");
        printf("| 3. Urgencia                   |\n");
        printf("| 4. Nombre Comision            |\n");
        printf("| 0. Volver                     |\n");
        scanf(" %c", &opt);

        switch (opt) {
            case '1': EditarParlamentarios((*Comision)->parlamentarios, (*Comision)->PlibrePoliticos); break;
            case '2': modificarString((*Comision)->informe); break;
            case '3': modificarString((*Comision)->Urgencia); break;
            case '4': modificarString((*Comision)->nombreComision); break;
            case '0': printf("Saliendo...\n"); break;
            default: printf("Opcion invalida, intente nuevamente.\n"); break;
        }
    } while (opt != '0');
}

void AgregarVoto(struct datosVoto **ListaVotos, int *Plibre, struct TramitacionLegislativa *TL) {
    char Rut[20], Camara[20];
    struct politico *Buscado;
    printf("|---Escriba Rut y camara del politico que desea agregar---|\n");
    scanf("%s %s", Rut, Camara);
    if (strcmp(Camara, "Diputados") == 0) {
        if (BuscarPolitico(TL->CamaraDeDiputados, Rut) == 0) {
            printf("|---Rut no encontrado---|\n");
        } else {
            Buscado = BuscarPolitico(TL->CamaraDeDiputados, Rut);
            ListaVotos[*Plibre]->votante = Buscado;
            (*Plibre)++;
        }
    }
}

void EliminarVoto(struct datosVoto **ListaVotos, int *Plibre) {
    char Rut;
    int i, j;
    printf("|---Ingrese rut que desea modificar---|\n");
    scanf("%s", &Rut);
    for (i = 0; i < (int)Plibre; i++) {
        if (strcmp(ListaVotos[i]->votante->rut, (const char *)Rut) == 0) {
            printf("|---Rut encontrado, Procediendo a Eliminar---|\n");
            ListaVotos[i] = NULL;
            for(j = i; (int*)j < Plibre; j++) ListaVotos[j] = ListaVotos[j+1];
            (*Plibre)--;
        }
    }
}

void ModificarListaVotaciones(struct datosVoto **ListaVotos, int *Plibre, struct TramitacionLegislativa *TL) {
    char opt;
    if (*ListaVotos == NULL) (*ListaVotos) = (struct datosVoto *)malloc(sizeof(struct datosVoto));

    do {
        printf("|---Que desea modificar?---|\n");
        printf("| 1. Agregar               |\n");
        printf("| 2. Eliminar              |\n");
        printf("| 0. Salir                 |\n");
        scanf(" %c", &opt);

        switch (opt) {
            case '1': AgregarVoto(ListaVotos, Plibre, TL); break;
            case '2': EliminarVoto(ListaVotos, Plibre); break;
            case '0': printf("Saliendo del menu de edicion...\n"); break;
            default: printf("Opcion invalida, intente nuevamente.\n"); break;
        }
    } while (opt != '0');
}

void ModificarFase(int *Plibre) {
    int fase;
    do {
        printf("|---Escriba a la fase que desea cambiar---|\n");
        scanf("%d", &fase);
        if (fase != 2 && fase != 1) printf("|---Fase no seleccionable, intente nuevamente---|\n");
    } while(fase != 2 && fase != 1);
    (*Plibre) = fase;
}

void ModificarVotacion(struct votaciones **Votaciones, struct TramitacionLegislativa *TL) {
    char opt;
    do {
        printf("|---Que votacion desea modificar?---|\n");
        printf("| 1. Fase de Votacion               |\n");
        printf("| 2. Lista de Votaciones            |\n");
        printf("| 0. Salir                          |\n");
        scanf(" %c", &opt);

        switch (opt) {
            case '1': ModificarFase(&(*Votaciones)->PlibreVotaciones); break;
            case '2': ModificarListaVotaciones((*Votaciones)->votos, (&(*Votaciones)->PlibreVotaciones), TL); break;
            case '0': printf("Saliendo del menu de edicion...\n"); break;
            default: printf("Opcion invalida, intente nuevamente.\n"); break;
        }
    } while (opt != '0');
}

void MenuVotaciones(struct votaciones **Votaciones[3], struct TramitacionLegislativa *TL) {
    char opt;
    do {
        printf("|---Que votacion desea modificar?---|\n");
        printf("| 1. Primera votacion camara origen |\n");
        printf("| 2. Segunda votacion camara origen |\n");
        printf("| 3. Primera votacion camara rev.   |\n");
        printf("| 4. Segunda votacion camara rev.   |\n");
        printf("| 0. Salir                          |\n");
        scanf(" %c", &opt);

        switch (opt) {
            case '1': ModificarVotacion(Votaciones[0], TL); break;
            case '2': ModificarVotacion(Votaciones[1], TL); break;
            case '3': ModificarVotacion(Votaciones[2], TL); break;
            case '4': ModificarVotacion(Votaciones[3], TL); break;
            case '0': printf("Saliendo del menu de edicion...\n"); break;
            default: printf("Opcion invalida, intente nuevamente.\n"); break;
        }
    } while (opt != '0');
}

void MenuProcesoDeAprobacion(struct ProcesoDeAprobacion **Proceso) {
    char opt;
    do {
        printf("|---Que area desea modificar?---|\n");
        printf("| 1. Nombre del presidente      |\n");
        printf("| 2. Fecha                      |\n");
        printf("| 3. Publicacion                |\n");
        printf("| 4. Tipo Publicacion           |\n");
        printf("| 5. Control Constitucional     |\n");
        printf("| 0. Salir                      |\n");
        scanf(" %c", &opt);

        switch (opt) {
            case '1': modificarString((*Proceso)->nombrePresidente); break;
            case '2': modificarString((*Proceso)->fecha); break;
            case '3': ModificarFase(&(*Proceso)->Publicacion); break;
            case '4': modificarString((*Proceso)->TipoPublicacion); break;
            case '5': ModificarFase(&(*Proceso)->ControlConstitucional); break;
            case '0': printf("Saliendo del menu de edicion...\n"); break;
            default: printf("Opcion invalida, intente nuevamente.\n"); break;
        }
    } while (opt != '0');
}

void MenuEditarProceso(struct datosProyectoLey **Ley, struct TramitacionLegislativa *TL) {
    char opt;
    do {
        printf("|---Que area desea modificar?---|\n");
        printf("| 1. Camara de Origen           |\n");
        printf("| 2. Camara Revisora            |\n");
        printf("| 3. Comision Mixta             |\n");
        printf("| 4. Votaciones                 |\n");
        printf("| 5. Proceso de aprobacion      |\n");
        printf("| 0. Salir                      |\n");
        scanf(" %c", &opt);

        switch (opt) {
            case '1': ModificarComision(&(*Ley)->CamaraDeOrigen); break;
            case '2': ModificarComision(&(*Ley)->CamaraRevisoria); break;
            case '3': ModificarComision(&(*Ley)->ComisionMixta); break;
            case '4': MenuVotaciones((*Ley)->VotacionLey, TL); break;
            case '5': MenuProcesoDeAprobacion(&(*Ley)->ProcesoAprobatorio); break;
            case '0': printf("Saliendo del menu de edicion...\n"); break;
            default: printf("Opcion invalida, intente nuevamente.\n"); break;
        }
    } while (opt != '0');
}

void MenuFunciones(struct nodoProyectoLey *raiz, struct TramitacionLegislativa *TL) {
    int IDLey;
    struct datosProyectoLey *Respaldo;
    printf("---Ingrese ID del proyecto a aplicar---\n");
    scanf("%d", &IDLey);
    Respaldo = BuscarProyectoEnArbol(raiz, IDLey);
    if (Respaldo == NULL) {
        printf("|---ID de Proyecto no encontrado---|\n");
    } else {
        MenuEditarProceso(&Respaldo, TL);
    }
}

int ContarTotalDiputados(struct nodopolitico* head) {
    int diputados = 0;
    struct nodopolitico* actual;
    if (head == NULL) return 0;
    actual = head;
    do {
        if (strcmp(actual->datos->cargo, "Diputado") == 0) diputados++;
        actual = actual->sig;
    } while (actual != head);
    return diputados;
}

int ContarTotalSenadores(struct nodopolitico* head) {
    int senadores = 0;
    struct nodopolitico* actual;
    if (head == NULL) return 0;
    actual = head;
    do {
        if (strcmp(actual->datos->cargo, "Senador") == 0) senadores++;
        actual = actual->sig;
    } while (actual != head);
    return senadores;
}

void MenuCantidadPoliticos(struct nodopolitico* head) {
    char opcion;
    int diputados, senadores;

    do {
        printf("\n|-----------------------------------|\n");
        printf("|      Cantidad de Politicos        |\n");
        printf("|-----------------------------------|\n");
        printf("| 1. Contar Diputados               |\n");
        printf("| 2. Contar Senadores               |\n");
        printf("| 3. Contar Total de parlamentarios |\n");
        printf("| 4. Volver al Menu Principal       |\n");
        printf("|-----------------------------------|\n");
        printf("Seleccione una opcion: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '1':
                diputados = ContarTotalDiputados(head);
                printf("Cantidad de Diputados: %d\n", diputados);
                break;
            case '2':
                senadores = ContarTotalSenadores(head);
                printf("Cantidad de Senadores: %d\n", senadores);
                break;
            case '3':
                diputados = ContarTotalDiputados(head);
                senadores = ContarTotalSenadores(head);
                printf("Cantidad de parlamentarios: %d\n", diputados + senadores);
                break;
            case '4':
                printf("Volviendo al Menu Principal...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != '4');
}

void ListarPorProyecto(struct TramitacionLegislativa *TL) {
    int idProyecto, i;
    struct datosProyectoLey *proyecto;
    struct nodopolitico *politicoActual;
    printf("|---Escriba la ley que gustaria listar---|\n");
    scanf("%d", &idProyecto);
    proyecto = BuscarProyectoEnArbol(TL->ArboldeProyectos, idProyecto);
    if (proyecto == NULL) {
        printf("Proyecto de ley no encontrado.\n");
        return;
    }
    printf("Politicos involucrados en el proyecto de ley ID %d:\n", idProyecto);
    politicoActual = TL->BDPoliticos;
    while (politicoActual != NULL) {
        if (politicoActual->datos->ProyectosInvolucrados != NULL) {
            for (i = 0; i < politicoActual->datos->plibreLeyes; i++) {
                if (politicoActual->datos->ProyectosInvolucrados[i]->id == idProyecto) {
                    printf(" - %s (%s)\n", politicoActual->datos->nombre, politicoActual->datos->cargo);
                    break;
                }
            }
        }
        politicoActual = politicoActual->sig;
    }
}

void Menu(struct TramitacionLegislativa *TL) {
    char opt;
    do {
        printf("|------------------------------------------------------|\n");
        printf("|  Bienvenido al Programa de Tramitacion Legislativa   |\n");
        printf("|----------- Seleccione una opcion --------------------|\n");
        printf("| 1. Base de Datos Proyectos                           |\n");
        printf("| 2. Base de Datos de Politicos                        |\n");
        printf("| 3. Lista Camara de Diputados                         |\n");
        printf("| 4. Lista Camara de Senadores                         |\n");
        printf("| 5. Funciones Avanzadas en Proyectos                  |\n");
        printf("| 6. Calcular Cantidad de Politicos                    |\n");
        printf("| 7. Listar Politicos de un Proyecto especifico        |\n");
        printf("| 0. Cerrar programa                                   |\n");
        printf("|------------------------------------------------------|\n");
        printf("Seleccione una opcion: ");
        scanf(" %c", &opt);

        switch (opt) {
            case '1': printf("Accediendo a Base de Datos de Proyectos...\n"); MenuArbolDeProyectos(TL); break;
            case '2': printf("Accediendo a Base de Datos de Politicos...\n"); MenuBasededatosPolitiicos(TL); break;
            case '3': MenuCamara(TL, TL->CamaraDeDiputados); break;
            case '4': MenuCamara(TL, TL->CamaraDeSenadores); break;
            case '5': MenuFunciones(TL->ArboldeProyectos, TL); break;
            case '6': MenuCantidadPoliticos(TL->BDPoliticos); break;
            case '7': ListarPorProyecto(TL); break;
            case '0': printf("Saliendo del programa...\n"); break;
            default: printf("Opcion invalida, intente nuevamente.\n"); break;
        }
    } while (opt != '0');
}

int main() {
    struct TramitacionLegislativa *Tramitacion = malloc(sizeof(struct TramitacionLegislativa));
    Tramitacion->ArboldeProyectos = NULL;
    Tramitacion->BDPoliticos = NULL;
    Tramitacion->CamaraDeDiputados = NULL;
    Tramitacion->CamaraDeSenadores = NULL;
    Menu(Tramitacion);
    free(Tramitacion);
    return 0;
}