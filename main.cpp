#include <iostream>
#include <string>
#include <iomanip> // uso de setw()
#include <sstream>

using namespace std;

// definimos el Struct para almacenar los usuarios, productos y ventas

struct Usuario{
    string usuario;
    string pass;
    int tipo;
    int status;
};

struct Producto{
    int id;
    string producto;
    float pc;
    float pv;
    int existencias;
    int nivelReorden;
    int status;
};

struct Venta{
    string vendedor;
    string productos[100];
    int cantidad[100];
    float pc[100];
    float pv[100];
    int totalProductosVentas = 0;
};


// Prototipos funciones

int menuPrincipal();
void menuAdmin();
bool validarLogin(int tipoUsuario);
void validarInput();
void mostrarInventario();
void mostrarProductos(int tipoOrden);
bool altaProducto(string& nombreProducto, float& pc, float& pv, int& existencia, int& nivelReorden);
Producto* buscarProducto(const string& nombreProducto);
Usuario* buscarUsuario(const string& nombreUsuario);
string convertirMinus(string str);
void consultarProducto();
void modificarProducto();
void bajaProducto();
void mostrarMenuAdminCuentasUsuario();
void altaUsuario(Usuario* usuario, string nombreUsuario);
void bajaUsuario(Usuario* usuario, string nombreUsuario);
void modificarUsuario(Usuario* usuario, string nombreUsuario);
void consultarUsuario(Usuario* usuario, string nombreUsuario);
void mostrarCuentasUsuarios();
void hacerVenta();
void imprimirTicket(const Venta& venta);
void restarInventario(const int& id, const int& cantidad);
void reiniciarVenta(Venta& venta);
void corteCajaVendedor(const string& vendedor);
void corteCajaGeneral();
void limpiarConsola();
void ordernarNombre();
void ordernarId();
void mostrarEncabezadosProducto();
void mostrarInfoProducto(const Producto& producto);
void mostrarEncabezadosUsuario();
void mostrarInfoUsuario(const Usuario& usuario);
int contarUsuariosAdmin();
string formatoMoneda(float monto);
bool reactivarProducto(Producto* producto);
float solicitarNumeroAlUsuario(const string& mensajeEntrada, const float& valorMin,const float& valorMax, const string& mensajeError, bool cambiarSigno);

// variables globales

int totalUsuarios = 3;
Usuario usuarios[100] = {
    {"admin", "123", 1, 1}, 
    {"vend1", "123", 2, 1}, 
    {"vend2", "123", 2, 1}
};

int totalProductos = 5;
Producto productos[100] = {
    {1,"Agua",13.39,18.55,12,4,1},
    {2,"Leche",12.35,15.5,16,5,1},
    {3,"Huevos",22.4,30.39,20,7,1},
    {4,"Pan",5.5,7.95,18,6,1},
    {5,"Refresco",10.99,14.75,30,8,1}
};

int totalVentas = 0;
Venta ventas[100];

string currentUser; // manejar al usuario que esta dentro del sistema

int main(){
    bool isExcecute = true;
    int optionMenu;
    while (isExcecute){
        optionMenu = menuPrincipal();
        switch (optionMenu){
            case 1:
                menuAdmin();
                break;
            case 2:
                hacerVenta();
                break;
            case 3:
                cout << "\n-------------------------------------------------\n     SALIENDO DEL SISTEMA. FIN DEL PROGRAMA.\n-------------------------------------------------\n\n";
                isExcecute = false;
                break;
            default:
                limpiarConsola();
                cout << "\n*** Opcion invalida. Intenta de nuevo. ***";
                break;
        }
    }
    return 0;
}

void limpiarConsola(){
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // macOS/Linux
    #endif
};

int menuPrincipal(){
    int option;
    cout << "\n\n\tMENU PRINCIPAL\n\n";
    cout << "1. Administrador\n2. Ventas\n3. Salir\n\n";
    cout << "\tOpcion: "; cin >> option;
    validarInput();
    return option;
}

void menuAdmin(){
    if (validarLogin(1)){
        limpiarConsola();
        int option;
        bool ejecutarMenu = true;
        string nombreProducto;
        float pc, pv;
        int existencia, nivelReorden;
        
        while(ejecutarMenu){
            cout << "\n\t\t\t\t\tUsuario: " << currentUser; //TODO: implementar en todos los menus?
            cout << "\n\n\tMENU ADMINISTRADOR\n\n";
            cout << "1. Altas\n2. Bajas\n3. Consultas\n4. Modificaciones\n5. Mostrar Inventario\n6. Administracion de Cuentas de Usuario\n7. Corte de caja general\n8. Regresar al menu anterior.\n\n";
            cout << "\tOpcion: ";
            cin >> option;
            validarInput();
            limpiarConsola();
            switch (option){
                case 1:
                    while(true){
                        cout << "\n\n\tALTA DE PRODUCTO\n\n";
                        cout << "Producto: "; cin >> nombreProducto;
                        if (nombreProducto == "*"){limpiarConsola(); break;}

                        Producto* producto = buscarProducto(nombreProducto);
                        if(producto != nullptr){
                            if(producto->status == 0){
                                if(reactivarProducto(producto)){
                                    mostrarEncabezadosProducto();
                                    mostrarInfoProducto(*producto);
                                    cout << "\n\n*** El producto " << producto->producto << " se dio de alta. ***\n\n";
                                }
                            }else{
                                cout << "\n\n*** El producto \"" << nombreProducto << "\"  ya existe. Intenta de nuevo. ***\n\n\n";
                            }
                            continue;
                        }

                        pc = solicitarNumeroAlUsuario("Precio compra: ", 00.00, 99999.99, "\n\n*** Error en el precio. ***\n\n\n", false);
                        pv = solicitarNumeroAlUsuario("Precio venta: ", pc, 99999.99, "\n\n*** El Precio de Venta no puede ser menor al Precio de Compra. Intenta de nuevo. ***\n\n\n", true);
                        existencia = solicitarNumeroAlUsuario("Existencia: ", 00.00, 99999.99, "\n\n*** Error en la cantidad. ***\n\n\n", false);
                        nivelReorden = solicitarNumeroAlUsuario("Nivel de Reorden: ", 00.00, existencia,"\n\n*** El Nivel de Reorden no puede ser mayor que la Existencia. Intenta de nuevo. ***\n\n\n", false);
                        
                        if(altaProducto(nombreProducto,pc,pv,existencia,nivelReorden)){
                            cout << "\n\nEl producto \"" << nombreProducto << "\" se agrego correctamente.\n\n";
                        }
                    }
                    break;
                case 2:                
                    // TODO: bajaProducto() - Pasar producto por parametro    
                    bajaProducto();
                    break;
                case 3:
                    // TODO: consultarProducto() - Pasar producto por parametro
                    consultarProducto();
                    break;
                case 4:
                    // TODO: modificarProducto() - Pasar producto por parametro
                    modificarProducto();
                    break;
                case 5:
                    mostrarInventario();
                    break;
                case 6:
                    mostrarMenuAdminCuentasUsuario();
                    break;
                case 7:
                    corteCajaGeneral();
                    break;
                case 8:
                    ejecutarMenu = false;
                    break;
                default:
                    cout << "\n*** Opcion invalida. Intenta de nuevo. ***";
                    break;
            }
        }
    }
}

bool reactivarProducto(Producto* producto){
    char res;
    cout << "\nEste producto estaba dado de baja, ¿Quieres darlo de alta nuevamente? (y/n): "; cin >> res; cout << '\n';
    if(tolower(res) == 'y'){
        ordernarId(); // ordenamos la lista para poderla modificar.
        productos[producto->id - 1].status = 1;
        return true;
    }
    return false;
}

float solicitarNumeroAlUsuario(const string& mensajeEntrada, const float& valorMin,const float& valorMax, const string& mensajeError, bool cambiarSigno){
    float valor;
    if(!cambiarSigno)
        do{
            cout << mensajeEntrada; cin >> valor; validarInput();
            if(valor <= valorMin){ cout << "\n\n*** Datos invalidos. Intenta de nuevo. ***\n\n\n"; continue; }
            if(valor > valorMax){ cout << mensajeError; }
        }while(valor <= valorMin || valor > valorMax);
    else{
        do{
            cout << mensajeEntrada; cin >> valor; validarInput();
            if(valor <= 0){ cout << "\n\n*** Datos invalidos. Intenta de nuevo. ***\n\n\n"; continue; }
            if(valor < valorMin){ cout << mensajeError; }
        }while(valor <= 0 || valor < valorMin);
    }
    return valor;
}

bool validarLogin(int tipoUsuario){
    bool isValid = false;
    string usuario, pass;
    do{
        cout << "\nUsuario: ";
        cin >> usuario;
        if(usuario == "*"){limpiarConsola(); break;}
        cout << "\nPassword: ";
        cin >> pass;

        for (int i = 0; i < totalUsuarios; i++){
            if (usuarios[i].usuario == usuario && usuarios[i].pass == pass && usuarios[i].tipo == tipoUsuario && usuarios[i].status == 1){
                currentUser = usuario;
                isValid = true;
                break;
            }
        }
        if (isValid == false) cout << "\n\n*** Usuario o contraseña incorrectos. Intenta de nuevo. ***" << endl;
    } while (!isValid);
    return isValid;
}

void validarInput(){
    // si el usuario ingresa un dato no numerico, se limpia el cache de cin.
    if (!cin.good()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void mostrarInventario(){
    int option;
    
    while(true){
        cout << "\n\n\tMOSTRAR INVENTARIO\n\nOrdenado por:\n";
        cout << "1. Por Id\n2. Por producto\n3. Regresar al menú anterior.\n\n";
        cout << "\tOpcion: "; cin >> option;
        validarInput();
        limpiarConsola();
        if(option == 1 || option == 2) {
            mostrarProductos(option);
            continue;
        }

        if(option == 3){ break; }

        cout << "\n*** Opcion invalida. Intenta de nuevo. ***";
    }
}

// Bubble Sort
void ordernarNombre(){
    Producto tmp;
    for(int i=0; i < totalProductos - 1; i++){
        for(int j=0; j < totalProductos - i - 1; j++){
            if(convertirMinus(productos[j].producto) > convertirMinus(productos[j + 1].producto)){
                tmp = productos[j];
                productos[j] = productos[j + 1];
                productos[j + 1] = tmp;
            }
        }
    }
}

void ordernarId(){
    Producto tmp;
    for(int i=0; i < totalProductos - 1; i++){
        for(int j=0; j < totalProductos - i - 1; j++){
            if(productos[j].id > productos[j + 1].id){
                tmp = productos[j];
                productos[j] = productos[j + 1];
                productos[j + 1] = tmp;
            }
        }
    }
}

void mostrarProductos(int tipoOrden){
    if(tipoOrden == 1)
        ordernarId();
    else
        ordernarNombre();
    
    cout << "---------------------------------------------------------------------------\n\t\t\t\tINVENTARIO\n---------------------------------------------------------------------------";
    mostrarEncabezadosProducto();
    for (int i = 0; i < totalProductos; i++){
        if(productos[i].status == 1){
            mostrarInfoProducto(productos[i]);
        }
    }
}

bool altaProducto(string& nombreProducto, float& pc, float& pv, int& existencia, int& nivelReorden){
    productos[totalProductos].id = totalProductos + 1;
    productos[totalProductos].producto = nombreProducto;
    productos[totalProductos].pc = pc;
    productos[totalProductos].pv = pv;
    productos[totalProductos].existencias = existencia;
    productos[totalProductos].nivelReorden = nivelReorden;
    productos[totalProductos].status = 1;
    totalProductos++; // se incrementa en 1 la cantidad de productos.
    return true;
}

Producto* buscarProducto(const string& nombreProducto){
    for(int i = 0; i < totalProductos; i++){
        if(convertirMinus(productos[i].producto) == convertirMinus(nombreProducto)){
            return &productos[i];
        }
    }
    return nullptr;
}

Usuario* buscarUsuario(const string& nombreUsuario){
    for(int i = 0; i < totalUsuarios; i++){
        if(convertirMinus(usuarios[i].usuario) == convertirMinus(nombreUsuario)){
            return &usuarios[i];
        }
    }
    return nullptr;
}

string convertirMinus(string str){
    for (char &c : str) c = tolower(c);
    return str;
}

void consultarProducto(){
    Producto* producto;
    string nombreProducto;
    while (true){
        cout << "\n\n\tCONSULTA\n\nProducto: "; cin >> nombreProducto;
        if (nombreProducto == "*"){limpiarConsola(); break;}
        producto = buscarProducto(nombreProducto);
        if(producto->status == 1){
            mostrarEncabezadosProducto();
            mostrarInfoProducto(*producto);
            continue;
        }
        cout << "\n\n*** No se encontro el producto \"" << nombreProducto << "\" ***\n\n";
    }
}

void mostrarEncabezadosProducto(){
    cout << "\n" << left << setw(5) << "Id"
            << setw(15) << "Producto"
            << setw(10) << "PC"
            << setw(10) << "PV"
            << setw(15) << "Existencias"
            << setw(20) << "Nivel de Reorden"
            << "Resurtir" << endl;
}

void mostrarInfoProducto(const Producto& producto){
    char resurtir;
    resurtir = (producto.existencias <= producto.nivelReorden) ? '*' : ' ';
    cout << left << setw(5) << producto.id
            << setw(15) << producto.producto
            << setw(10) << producto.pc
            << setw(10) << producto.pv
            << setw(15) << producto.existencias
            << setw(20) << producto.nivelReorden
            << resurtir << endl;
}

void modificarProducto(){
    Producto* producto;
    string nombreProducto;
    float pc, pv;
    int existencia, nivelReorden, opcion;

    ordernarId(); // ordenamos la lista para poderla modificar.
    
    while (true){
        bool mostrarOpciones = true;
        cout << "\n\n\tMODIFICACIONES\n\nProducto: "; cin >> nombreProducto;
        if (nombreProducto == "*"){limpiarConsola(); break;}
        producto = buscarProducto(nombreProducto);
        if(producto->status == 1){
            limpiarConsola();
            while (mostrarOpciones){
                    cout << "\n\n\tMODIFICACIONES\n\nProducto: "<< producto->producto << endl;
                    mostrarEncabezadosProducto();
                    mostrarInfoProducto(*producto);
                    cout << "\n1. Precio de compra\n2. Precio de venta\n3. Existencias\n4. Nidel de reorden\n5. Regresar al menu anterior\n\n";
                    cout << "\tOpcion: "; cin >> opcion; validarInput();
                    switch(opcion){
                        case 1:
                            cout << "\nPC actual: "<< producto->pc;
                            cout << "\nPC nuevo: "; cin >> pc; validarInput();
                            productos[producto->id - 1].pc = pc; // actualizamos el producto original con el ID.
                            producto = buscarProducto(nombreProducto); // actualizamos nuestro producto temporal.
                            cout << "\n\n*** Precio de compra actualizado ***\n\n";
                            break;
                        case 2:
                            cout << "\nPV actual: "<< producto->pv;
                            cout << "\nPV nuevo: "; cin >> pv; validarInput();
                            productos[producto->id - 1].pv = pv; // actualizamos el producto original con el ID.
                            producto = buscarProducto(nombreProducto); // actualizamos nuestro producto temporal.
                            cout << "\n\n*** Precio de venta actualizado ***\n\n";
                            break;
                        case 3:
                            cout << "\nExistencias actuales: "<< producto->existencias;
                            cout << "\nExistencias nuevas: "; cin >> existencia; validarInput();
                            productos[producto->id - 1].existencias = existencia; // actualizamos el producto original con el ID.
                            producto = buscarProducto(nombreProducto); // actualizamos nuestro producto temporal.
                            cout << "\n\n*** Existencia actualizada ***\n\n";
                            break;
                        case 4:
                            cout << "\nNivel de reorden actual: "<< producto->nivelReorden;
                            cout << "\nNivel de reorden nuevo: "; cin >> nivelReorden; validarInput();
                            productos[producto->id - 1].nivelReorden = nivelReorden; // actualizamos el producto original con el ID.
                            producto = buscarProducto(nombreProducto); // actualizamos nuestro producto temporal.
                            cout << "\n\n*** Nivel de reorden actualizado ***\n\n";
                            break;
                        case 5:
                            limpiarConsola();
                            mostrarOpciones = false;
                            break;
                        default:
                            cout << "*** Opcion incorrecta. Intenta de nuevo. ***";
                            break;
                    }
                }
        } else { limpiarConsola(); cout << "\n\n*** No se encontro el producto \"" << nombreProducto << "\" ***\n\n"; }
    }
}

void bajaProducto(){
    Producto* producto;
    string nombreProducto;

    ordernarId(); // ordenamos la lista para poderla modificar.

    while(true){
        cout << "\n\n\tBAJA DE PRODUCTO\n\n";
        cout << "Producto: "; cin >> nombreProducto;
        if (nombreProducto == "*"){limpiarConsola(); break;}

        producto = buscarProducto(nombreProducto);
        if(producto->status == 0){cout << "\n\n***Producto \"" << nombreProducto << "\" no encontrado. Intenta de nuevo. ***\n\n"; continue;}

        productos[producto->id - 1].status = 0;
        cout << "El producto \"" << producto->producto << "\" se dio de baja\n";
    }
}

void mostrarMenuAdminCuentasUsuario(){
    int option;
    bool ejecutarMenu = true;
    string nombreUsuario;

    while(ejecutarMenu){
        cout << "\n\n\tMENU ADMINISTRACION DE CUENTAS DE USUARIOS\n\n";
        cout << "1. Altas\n2. Bajas\n3. Consultas\n4. Modificaciones\n5. Mostrar cuentas de usuarios\n6. Regresar al menu anterior.\n\n";
        cout << "\tOpcion: ";
        cin >> option;
        validarInput();
        limpiarConsola();
        switch (option){
            case 1:
                while(true){
                    cout << "\n\n\tALTA USUARIO\n\n";
                    cout << "Usuario: "; cin >> nombreUsuario;
                    if (nombreUsuario == "*"){limpiarConsola(); break;}
                    Usuario* usuario = buscarUsuario(nombreUsuario);
                    altaUsuario(usuario, nombreUsuario);
                }
                break;
            case 2:
                while(true){
                    cout << "\n\n\tBAJA USUARIO\n\n";
                    cout << "Usuario: "; cin >> nombreUsuario;
                    if (nombreUsuario == "*"){limpiarConsola(); break;}
                    Usuario* usuario = buscarUsuario(nombreUsuario);
                    bajaUsuario(usuario, nombreUsuario);
                }
                break;
            case 3:
                while(true){
                    cout << "\n\n\tCONSULTA USUARIO\n\n";
                    cout << "Usuario: "; cin >> nombreUsuario;
                    if (nombreUsuario == "*"){limpiarConsola(); break;}
                    Usuario* usuario = buscarUsuario(nombreUsuario);
                    consultarUsuario(usuario, nombreUsuario);
                }
                break;
            case 4:
                while(true){
                    cout << "\n\n\tMODIFICACIONES\n\n";
                    cout << "Usuario: "; cin >> nombreUsuario;
                    if (nombreUsuario == "*"){limpiarConsola(); break;}
                    Usuario* usuario = buscarUsuario(nombreUsuario);
                    modificarUsuario(usuario, nombreUsuario);
                }
                break;
            case 5:
                mostrarCuentasUsuarios();
                break;
            case 6:
                ejecutarMenu = false;
                break;
            default:
                cout << "\n*** Opcion invalida. Intenta de nuevo. ***";
                break;
        }
    }
}

void altaUsuario(Usuario* usuario, string nombreUsuario){
    string pass;
    int tipoUsuario;
    
    if(usuario == nullptr){
        cout << "Contraseña: "; cin >> pass;
        do{
            cout << "Tipo (1 admin / 2 vendedor): "; cin >> tipoUsuario; validarInput();
            if(tipoUsuario != 1 && tipoUsuario != 2) { cout << "\n\n*** Datos invalidos. Intenta de nuevo. ***\n\n\n"; }
        }while(tipoUsuario != 1 && tipoUsuario != 2);
        // se agrega el usuario.
        usuarios[totalUsuarios].usuario = nombreUsuario;
        usuarios[totalUsuarios].pass = pass;
        usuarios[totalUsuarios].tipo = tipoUsuario;
        usuarios[totalUsuarios].status = 1;
        totalUsuarios++; // se incrementa en 1 la cantidad de usuarios.
        
        string tipoUsuarioStr = (tipoUsuario == 2) ? "Vendedor" : "Admin";
        cout << "\n\nEl Usuario \"" << nombreUsuario << "\" se agrego correctamente como " << tipoUsuarioStr << ".\n\n";
        return;
    }
    if(usuario->status == 1){
        cout << "\n\n*** El usuario \"" << usuario->usuario << "\" ya existe. Intenta de nuevo. ***\n\n"; 
        return;
    }else {
        char res;
        cout << "\nEl Usuario estaba dado de baja, ¿Quieres darlo de alta nuevamente? (y/n): "; cin >> res; cout << '\n';
        if(tolower(res) == 'y'){
            usuario->status = 1;
            mostrarEncabezadosUsuario();
            mostrarInfoUsuario(*usuario);
            cout << "\n\nEl usuario " << usuario->usuario << " se dio de alta.\n\n";
        }
    }
}

void bajaUsuario(Usuario* usuario, string nombreUsuario){
    if(usuario == nullptr || usuario->status == 0){
        cout << "\n\n*** No se encontro el usuario \"" << nombreUsuario << "\" ***\n";
        return;
    }
    if(usuario->status == 1){
        char res;
        cout << "\nEstas seguro que quieres darlo de baja? (y/n): "; cin >> res; cout << '\n';
        if(tolower(res) == 'y'){
            if(usuario->tipo == 1){ // 1 = admin
                cout << "El usuario es admin, por favor vuelve a confirmar (y/n): "; cin >> res; cout << '\n';
                if(tolower(res) == 'y'){
                    if(contarUsuariosAdmin() == 1){
                        cout << "\n\n*** Error! Debe existir por lo menos un admin. ***\n\n";
                        return;
                    }
                    usuario->status = 0; 
                    cout << "El usuario \"" << usuario->usuario << "\" se dio de baja\n";
                }
            }
            else{
                usuario->status = 0; 
                cout << "El usuario \"" << usuario->usuario << "\" se dio de baja\n";
            }
        }
    }
}

int contarUsuariosAdmin(){
    int totalAdmin = 0;
    for(int i=0; i < totalUsuarios; i++){
        if(usuarios[i].tipo == 1 && usuarios[i].status == 1) totalAdmin++;
    }
    return totalAdmin;
}

void modificarUsuario(Usuario* usuario, string nombreUsuario){
    if(usuario == nullptr || usuario->status == 0){
        cout << "\n\n***Usuario \"" << nombreUsuario << "\" no encontrado. Intenta de nuevo. ***\n\n";
        return;
    }
    string pass;
    int opcion, tipoUsuario;
    bool mostrarOpciones = true;
    limpiarConsola();
    while(mostrarOpciones){
        cout << "\n\n\tMODIFICACIONES\n\nUsuario: "<< nombreUsuario << endl;
        mostrarEncabezadosUsuario();
        mostrarInfoUsuario(*usuario);
        cout << "\n1. Contraseña\n2. Tipo\n3. Regresar al menu anterior\n\n";
        cout << "\tOpcion: "; cin >> opcion; validarInput();
        switch(opcion){
            case 1:
            cout << "\nContraseña actual: "<< usuario->pass;
            cout << "\nContraseña nueva: "; cin >> pass;
            usuario->pass = pass; // actualizamos el password
            limpiarConsola();
            cout << "\n\n\tContraseña actualizada\n\n";
            break;
            case 2:
                do{
                    cout << "\nTipo actual: "<< usuario->tipo;
                    cout << "\nTipo nuevo: "; cin >> tipoUsuario; validarInput();
                    if(tipoUsuario != 1 && tipoUsuario != 2) { cout << "\n\n*** Datos invalidos. (1 admin / 2 vendedor). Intenta de nuevo. ***\n\n\n"; }
                }while(tipoUsuario != 1 && tipoUsuario != 2);
                // solo se puede modificar el tipo del admin si existen mas de uno
                if(usuario->tipo == 1 && tipoUsuario == 2){
                    if(contarUsuariosAdmin() == 1){
                        limpiarConsola();
                        cout << "\n\n*** Error! Debe existir por lo menos un admin. ***\n\n";
                        break;
                    }
                }
                usuario->tipo = tipoUsuario;
                limpiarConsola();
                cout << "\n\n\tTipo de Usuario actualizado\n\n";
                break;
            case 3:
                limpiarConsola();
                mostrarOpciones = false;
                break;
            default:
                limpiarConsola();
                cout << "*** Opcion incorrecta. Intenta de nuevo. ***";
                break;
        }
    }
}

void mostrarEncabezadosUsuario(){
    cout << "\n" << left 
        << setw(15) << "Usuario"
        << setw(10) << "Pass"
        << setw(10) << "Tipo"
        << "St" << endl;
}

void mostrarInfoUsuario(const Usuario& usuario){
    cout << left 
        << setw(15) << usuario.usuario
        << setw(10) << usuario.pass
        << setw(10) << usuario.tipo
        << usuario.status << endl;
}


void consultarUsuario(Usuario* usuario, string nombreUsuario){
    if(usuario == nullptr || usuario->status == 0){
        cout << "\n\n*** No se encontro el usuario \"" << nombreUsuario << "\" ***\n\n";
        return;
    }
    if(usuario->status == 1){
        mostrarEncabezadosUsuario();
        mostrarInfoUsuario(*usuario); // Dereferenciar el puntero
    }
}

void mostrarCuentasUsuarios(){
    cout << "---------------------------------------------------------------------------\n\t\t\tMOSTRAR CUENTAS DE USUARIOS\n---------------------------------------------------------------------------\n";
    mostrarEncabezadosUsuario();
    for(int i = 0; i < totalUsuarios; i++){
        if(usuarios[i].status == 1){
            mostrarInfoUsuario(usuarios[i]);
        }
    }
    cout << "\n\n";
}

void hacerVenta(){
    if (validarLogin(2)){
        limpiarConsola();
        Producto* producto;
        Venta ventaActual;
        string nombreProducto;
        int cantidad;
        
        cout << "\n\n\tVENTAS: " << currentUser <<"\n\n";
        while(true){
            cout << "Producto: "; cin >> nombreProducto;

            if(nombreProducto == "*"){ // imprimir ticket
                limpiarConsola(); 
                ventas[totalVentas] = ventaActual; // agregamos la venta a la lista
                imprimirTicket(ventaActual); 
                totalVentas++; // se incrementa en uno las ventas
                reiniciarVenta(ventaActual); // se vuelve a inicializar vacio
                continue;
            }
            if(nombreProducto == "**"){ // corte de caja vendedor
                limpiarConsola();
                corteCajaVendedor(currentUser);
                break;
            }

            producto = buscarProducto(nombreProducto);
            // validamos que el producto exista o no este dado de baja
            if(producto->status == 0){ cout << "\n\n*** No se encontro el producto. Intenta de nuevo ***\n\n"; continue; }
            // validamos que tengan existencias
            if(producto->existencias <= 0){ cout << "\n\n*** El producto no tiene existencia. Intenta de nuevo ***\n\n"; continue; }

            cout << "Cantidad: "; cin >> cantidad; validarInput();
            if(cantidad <= 0){cout << "\n\n*** Cantidad invalida. Intenta de nuevo ***\n\n"; continue;}

            // validamos que tengamos las existencias que solicita el cliente
            if(producto->existencias < cantidad){
                string resp;
                cout << "\n\nNo hay " << cantidad << ", solo hay " << producto->existencias << ", ¿realizar la venta de " << producto->existencias << "? ('Y' para aceptar / cualquiera para omitir): ";
                cin >> resp;
                if( convertirMinus(resp) != "y") { cout << "\n"; continue; }
                cantidad = producto->existencias;
                cout << "Se agregaron " << producto->existencias << endl;
            }
            
            // creamos la venta dentro de la estructura de Ventas
            ventaActual.vendedor = currentUser; // vendedor loggeado
            ventaActual.productos[ventaActual.totalProductosVentas] = producto->producto; // nombre del producto
            ventaActual.cantidad[ventaActual.totalProductosVentas] = cantidad;
            ventaActual.pc[ventaActual.totalProductosVentas] = producto->pc;
            ventaActual.pv[ventaActual.totalProductosVentas] = producto->pv;
            ventaActual.totalProductosVentas++;

            // Restar producto al inventario
            restarInventario(producto->id, cantidad);

            cout << "\n";
        }
    }
}

void imprimirTicket(const Venta& venta){
    float total = 0;
    cout << "\n\n-------------------------------------------------------\n\n";
    cout << "\t\tAbarrotes \"El Inge\"";
    cout << "\n\t\t Ticket de Venta\n\n\n";
    cout << "Vendedor: " << currentUser << endl;

    cout << "\n" << left << setw(15) << "Producto"
                    << setw(12) << "Cantidad"
                    << setw(20) << "Precio Unitario"
                    << "Subtotal" << endl;
    for(int i = 0; i < venta.totalProductosVentas; i++){
        float subtotal = venta.cantidad[i] * venta.pv[i];
        total += subtotal;
        cout << left << setw(15) << venta.productos[i]
                << setw(12) << venta.cantidad[i]
                << setw(20) << formatoMoneda(venta.pv[i])
                << formatoMoneda(subtotal) << endl;
    }
    cout << "\n\t\t\t\t     Total: $" << total << "\n\n-------------------------------------------------------\n\n";
}

string formatoMoneda(float monto){
    ostringstream oss;
    oss << "$" << fixed << setprecision(2) << monto;
    return oss.str();
}

void reiniciarVenta(Venta& venta) {
    venta.vendedor = "";
    for (int i = 0; i < venta.totalProductosVentas; i++) {
        venta.productos[i] = "";
        venta.cantidad[i] = 0;  
        venta.pc[i] = 0.0;      
        venta.pv[i] = 0.0;
    }
    venta.totalProductosVentas = 0;
}

void restarInventario(const int& id, const int& cantidad){
    for(int i=0; i<totalProductos; i++){
        if(productos[i].id == id){
            productos[i].existencias -= cantidad;
            break;
        }
    }
}

void corteCajaVendedor(const string& vendedor){
    float ingresos = 0, egresos = 0;
    
    cout << "\n\n-------------------------------------------------------\n\n";
    cout << "\t\tCorte de Caja: " << vendedor << "\n\n";
    for(int i=0; i < totalVentas; i++){
        if(ventas[i].vendedor == vendedor){
            for(int k=0; k < ventas[i].totalProductosVentas; k++){
                ingresos += (ventas[i].pv[k] * ventas[i].cantidad[k]);
                egresos  += (ventas[i].pc[k] * ventas[i].cantidad[k]);
            }
        }
    }
    cout << "Ingresos: $" << ingresos << endl;
    cout << "Egresos:  $" << egresos << endl;
    cout << "\nUtilidad: $" << ingresos - egresos << endl;
    cout << "\n-------------------------------------------------------\n\n";
}

void corteCajaGeneral(){
    float ingresos = 0, egresos = 0;
    cout << "\n\n-------------------------------------------------------\n\n";
    cout << "\t\tCorte general\n\n";
    for(int i=0; i<totalVentas; i++){
        for(int j=0; j < ventas[i].totalProductosVentas; j++){
            ingresos += (ventas[i].pv[j] * ventas[i].cantidad[j]);
            egresos  += (ventas[i].pc[j] * ventas[i].cantidad[j]);
        }
    }
    cout << "Ingresos: $" << ingresos << endl;
    cout << "Egresos:  $" << egresos << endl;
    cout << "\nUtilidad: $" << ingresos - egresos << endl;
    cout << "\n-------------------------------------------------------\n\n";
}