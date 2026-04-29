#include <iostream>
#include <string>
#include <vector>

using namespace std;

// UTILIDADES GENERALES

int leerOpcion() {
    int op;
    cin >> op;
    cin.ignore(1000, '\n');
    return op;
}

string leerTexto(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

string fechaHoy() {
    return "2026-04-29";
}

//  MODULO 1 – USUARIOS

class Usuario {
protected:
    int id;
    string cedula;
    string nombre;
    string email;
    string telefono;
    static int contadorId;

public:
    Usuario(const string& ced, const string& n, const string& e, const string& t)
        : id(++contadorId), cedula(ced), nombre(n), email(e), telefono(t) {}
    virtual ~Usuario() {}

    int getId() const { return id; }
    string getCedula() const { return cedula; }
    string getNombre() const { return nombre; }
    string getEmail() const { return email; }
    string getTelefono() const { return telefono; }

    virtual void mostrarInfo() const = 0;
    virtual string getTipo() const = 0;

};
int Usuario::contadorId = 0;

class Cliente : public Usuario {
private:
    string direccion;
    vector<int> historialPedidosIds;

public:
    Cliente(const string& ced, const string& n, const string& e, const string& t, const string& d)
        : Usuario(ced, n, e, t), direccion(d) {}

    void setDireccion(const string& d) { direccion = d; }
    string getDireccion() const { return direccion; }

    void agregarPedido(int pedidoId) { historialPedidosIds.push_back(pedidoId); }
    const vector<int>& getHistorial() const { return historialPedidosIds; }

    string getTipo() const override { return "Cliente"; }

    void mostrarInfo() const override {
        cout << "  Cliente -> ID: " << id << " | Cedula: " << cedula 
             << " | Nombre: " << nombre << " | Email: " << email
             << " | Tel: " << telefono << " | Dir: " << direccion << "\n";
    }
};

// Empleado (clase base 2)
class Empleado : public Usuario {
protected:
    string cargo;
public:
    Empleado(const string& ced, const string& n, const string& e, const string& t, const string& c)
        : Usuario(ced, n, e, t), cargo(c) {}

    string getCargo() const { return cargo; }
    string getTipo()  const override { return "Empleado"; }

    void mostrarInfo() const override {
        cout << "  Empleado - ID: " << id << " | Cedula: " << cedula
             << " | Nombre: " << nombre << " | Cargo: " << cargo << "\n";
    }
};

// Vendedor
class Vendedor : public Empleado {
private:
    double porcentajeComision;
    double totalVendido;
    int    numVentas;
    vector<int> pedidosGestionados;

public:
    Vendedor(const string& ced, const string& n, const string& e, const string& t, double porcentaje = 5.0)
        : Empleado(ced, n, e, t, "Vendedor"), porcentajeComision(porcentaje), totalVendido(0.0), numVentas(0) {}

    double getPorcentajeComision() const { return porcentajeComision; }
    double getTotalVendido()       const { return totalVendido; }
    int    getNumVentas()          const { return numVentas; }

    double calcularComision() const {
        return totalVendido * (porcentajeComision / 100.0);
    }

    void registrarVenta(int pedidoId, double monto) {
        pedidosGestionados.push_back(pedidoId);
        totalVendido += monto;
        numVentas++;
    }

    void mostrarComisiones() const {
        cout.setf(ios::fixed); cout.precision(2);
        cout << "\n  ---- COMISIONES DE " << nombre << " ----\n"
             << "  Ventas realizadas   : " << numVentas << "\n"
             << "  Total vendido       : $" << totalVendido << "\n"
             << "  Porcentaje comision : " << porcentajeComision << "%\n"
             << "  Comision ganada     : $" << calcularComision() << "\n";
    }

    string getTipo() const override { return "Vendedor"; }

    void mostrarInfo() const override {
        cout.setf(ios::fixed); cout.precision(2);
        cout << "  Vendedor - ID: " << id << " | Cedula: " << cedula
             << " | Nombre: " << nombre << " | Comision: " << porcentajeComision 
             << "% | Total vendido: $" << totalVendido << "\n";
    }
};

// Administrador 
class Administrador : public Empleado {
private:
    string nivel;
public:
    Administrador(const string& ced, const string& n, const string& e, const string& t, const string& niv)
        : Empleado(ced, n, e, t, "Administrador"), nivel(niv) {}

    string getNivel() const { return nivel; }
    string getTipo()  const override { return "Administrador"; }

    void mostrarInfo() const override {
        cout << "  Admin    - ID: " << id << " | Cedula: " << cedula
             << " | Nombre: " << nombre << " | Email: " << email
             << " | Nivel: " << nivel << "\n";
    }
};

//  MODULO 2 – PRODUCTOS E INVENTARIO

class Producto {
private:
    int id;
    string nombre;
    string descripcion;
    double precio;
    int stock;
    string categoria;
    static int contadorId;

public:
    Producto(const string& n, const string& desc, double p, int s, const string& cat)
        : id(++contadorId), nombre(n), descripcion(desc), precio(p), stock(s), categoria(cat) {}

    int getId() const { return id; }
    string getNombre() const { return nombre; }
    double getPrecio() const { return precio; }
    int getStock() const { return stock; }
    string getCategoria() const { return categoria; }

    bool reducirStock(int cantidad) {
        if (cantidad <= 0 || cantidad > stock) return false;
        stock -= cantidad;
        return true;
    }

    void aumentarStock(int cantidad) {
        if (cantidad > 0) stock += cantidad;
    }

    void mostrar() const {
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "ID:" << id << " " << nombre << " | $" << precio << " | Stock: " << stock << " | Cat: " << categoria << "\n";
    }
};
int Producto::contadorId = 0;

class Inventario {
private:
    vector<Producto> productos;

    int buscarIndice(int id) const {
        for (int i = 0; i < (int)productos.size(); i++)
            if (productos[i].getId() == id) return i;
        return -1;
    }

public:
    void agregarProducto(const string& nombre, const string& desc, double precio, int stock, const string& categoria) {
        productos.emplace_back(nombre, desc, precio, stock, categoria);
        cout << " Producto '" << nombre << "' agregado al inventario\n";
    }

    Producto* buscarProducto(int id) {
        int idx = buscarIndice(id);
        if (idx == -1) return nullptr;
        return &productos[idx];
    }

    void listarProductos() const {
        cout << "\n----- INVENTARIO (" << productos.size() << ") -----\n";
        for (const auto& p : productos) p.mostrar();
    }

    int getTotalProductos() const {
        return (int)productos.size();
    }
};

//  MODULO 3 – CARRITO Y PEDIDO

class LineaPedido {
private:
    int productoId;
    string nombreProducto;
    double precioUnitario;
    int cantidad;

public:
    LineaPedido(int pid, const string& nombre, double precio, int cant)
        : productoId(pid), nombreProducto(nombre), precioUnitario(precio), cantidad(cant) {}

    double getSubtotal() const { return precioUnitario * cantidad; }
    int getProductoId() const { return productoId; }
    int getCantidad() const { return cantidad; }

    void mostrar() const {
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "   " << nombreProducto << " x" << cantidad
             << " C/U $" << precioUnitario
             << " =$" << getSubtotal() << "\n";
    }
};

class CarritoDeCompras {
private:
    Cliente* cliente;
    vector<Producto*> items;
    vector<int> cantidades;

public:
    explicit CarritoDeCompras(Cliente* c) : cliente(c) {}

    bool agregarItem(Producto* p, int cant) {
        if (!p || cant <= 0) return false;
        if (p->getStock() < cant) {
            cout << " Stock insuficiente: '" << p->getNombre() << "'\n";
            return false;
        }
        for (int i = 0; i < (int)items.size(); i++) {
            if (items[i]->getId() == p->getId()) {
                cantidades[i] += cant;
                cout << " Cantidad actualizada: '" << p->getNombre() << "'\n";
                return true;
            }
        }
        items.push_back(p);
        cantidades.push_back(cant);
        cout << " '" << p->getNombre() << "' agregado al carrito\n";
        return true;
    }

    double calcularTotal() const {
        double t = 0;
        for (int i = 0; i < (int)items.size(); i++)
            t += items[i]->getPrecio() * cantidades[i];
        return t;
    }

    void mostrarCarrito() const {
        cout << "\n----- CARRITO DE " << cliente->getNombre() << " -----\n";
        if (items.empty()) { cout << "  (vacio)\n"; return; }
        for (int i = 0; i < (int)items.size(); i++) {
            cout.setf(ios::fixed);
            cout.precision(2);
            cout << "  " << items[i]->getNombre()
                 << " x" << cantidades[i]
                 << " =$" << items[i]->getPrecio() * cantidades[i] << "\n";
        }
        cout << "  TOTAL: $" << calcularTotal() << "\n";
    }

    void vaciar() { items.clear(); cantidades.clear(); }
    bool estaVacio() const { return items.empty(); }

    Cliente* getCliente() const { return cliente; }
    const vector<Producto*>& getItems() const { return items; }
    const vector<int>& getCantidades() const { return cantidades; }
};

class Pedido {
public:
    enum class Estado { PENDIENTE, CONFIRMADO, ENVIADO, ENTREGADO, CANCELADO };

private:
    int id;
    Cliente* cliente;
    vector<LineaPedido> lineas;
    string fechaPedido;
    Estado estado;
    double total;
    static int contadorId;

    static string estadoStr(Estado e) {
        switch (e) {
            case Estado::PENDIENTE:  return "PENDIENTE";
            case Estado::CONFIRMADO: return "CONFIRMADO";
            case Estado::ENVIADO:    return "ENVIADO";
            case Estado::ENTREGADO:  return "ENTREGADO";
            case Estado::CANCELADO:  return "CANCELADO";
        }
        return "?";
    }

public:
    Pedido(Cliente* c, const string& fecha)
        : id(++contadorId), cliente(c), fechaPedido(fecha),
          estado(Estado::PENDIENTE), total(0.0) {}

    void agregarLinea(int pid, const string& nombre, double precio, int cant) {
        lineas.emplace_back(pid, nombre, precio, cant);
        total += precio * cant;
    }

    void cambiarEstado(Estado e) {
        estado = e;
        cout << " Pedido #" << id << " : " << estadoStr(e) << "\n";
    }

    void mostrar() const {
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "\n------PEDIDO #" << id << "\n"
             << "   Cliente : " << cliente->getNombre() << "\n"
             << "   Fecha   : " << fechaPedido << " Estado: " << estadoStr(estado) << "\n"
             << "   Lineas  :\n";
        for (const auto& l : lineas) l.mostrar();
        cout << "   TOTAL   : $" << total << "\n";
    }

    int getId() const { return id; }
    double getTotal() const { return total; }
    Estado getEstado() const { return estado; }
    Cliente* getCliente() const { return cliente; }

    // Si en el futuro se necesita, este getter expone las lineas:
    const vector<LineaPedido>& getLineas() const { return lineas; }
};
int Pedido::contadorId = 0;

//  MODULO 4 – PAGOS

class Pago {
protected:
    int id;
    double monto;
    string fechaPago;
    bool procesado;
    static int contadorId;

public:
    Pago(double monto, const string& fecha) : id(++contadorId), monto(monto), fechaPago(fecha), procesado(false) {}
    virtual ~Pago() {}

    virtual bool procesar() = 0;
    virtual void mostrarDetalle() const = 0;
    virtual string getTipoPago() const = 0;

    virtual void generarRecibo() const {
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "-----RECIBO #" << id << "\n"
             << "  Tipo   : " << getTipoPago() << "\n"
             << "  Monto  : $" << monto << "\n"
             << "  Fecha  : " << fechaPago << "\n"
             << "  Estado : " << (procesado ? "PROCESADO" : "PENDIENTE") << "\n";
    }
};
int Pago::contadorId = 0;

class PagoTarjeta : public Pago {
private:
    string ultimosDigitos;
    string nombreTitular;
    string tipoTarjeta;

public:
    PagoTarjeta(double monto, const string& fecha, const string& digitos, const string& titular, const string& tipo)
        : Pago(monto, fecha), ultimosDigitos(digitos), nombreTitular(titular), tipoTarjeta(tipo) {}

    bool procesar() override {
        cout << " Procesando " << tipoTarjeta << " terminada en " << ultimosDigitos << "...\n";
        procesado = true;
        cout << " Pago con tarjeta aprobado\n";
        return true;
    }
    void mostrarDetalle() const override {
        cout << "  " << tipoTarjeta << " ****" << ultimosDigitos
             << "  Titular: " << nombreTitular << "\n";
    }
    string getTipoPago() const override { return "Tarjeta " + tipoTarjeta; }
};

class PagoEfectivo : public Pago {
private:
    double montoPagado;
    double cambio;

public:
    PagoEfectivo(double monto, const string& fecha, double pagado)
        : Pago(monto, fecha), montoPagado(pagado) {
        if (pagado < monto) throw invalid_argument("Monto insuficiente para pago en efectivo");
        cambio = pagado - monto;
    }

    bool procesar() override {
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "  Efectivo recibido: $" << montoPagado
             << "  Cambio: $" << cambio << "\n";
        procesado = true;
        cout << " Pago en efectivo procesado\n";
        return true;
    }
    void mostrarDetalle() const override {
        cout << "  Efectivo | Pagado: $" << montoPagado
             << "  Cambio: $" << cambio << "\n";
    }
    string getTipoPago() const override { return "Efectivo"; }
};

class PagoDigital : public Pago {
private:
    string plataforma;
    string cuenta;

public:
    PagoDigital(double monto, const string& fecha, const string& plataforma, const string& cuenta)
        : Pago(monto, fecha), plataforma(plataforma), cuenta(cuenta) {}

    bool procesar() override {
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "  Enviando $" << monto
             << " via " << plataforma << " desde " << cuenta << "...\n";
        procesado = true;
        cout << " Pago digital confirmado\n";
        return true;
    }
    void mostrarDetalle() const override {
        cout << "  " << plataforma << " | Cuenta: " << cuenta << "\n";
    }
    string getTipoPago() const override { return "Digital - " + plataforma; }
};

//  MODULO 5 – GESTOR DE CLIENTES

class GestorClientes {
private:
    vector<Cliente*> clientes;

    int buscarIndice(int id) const {
        for (int i = 0; i < (int)clientes.size(); i++)
            if (clientes[i]->getId() == id) return i;
        return -1;
    }

public:
    ~GestorClientes() {
        for (auto c : clientes) delete c;
        clientes.clear();
    }

    Cliente* registrarCliente(const string& ced, const string& nombre, const string& email, const string& tel, const string& dir) {
        Cliente* c = new Cliente(ced, nombre, email, tel, dir);
        clientes.push_back(c);
        cout << " Cliente '" << nombre << "' registrado (ID " << c->getId() << ")\n";
        return c;
    }
    
    Cliente* buscarClientePorCedula(const string& cedula) const {
        for (auto c : clientes)
            if (c->getCedula() == cedula) return c;
        return nullptr;
    }

    bool cedulaExiste(const string& cedula) const {
        return buscarClientePorCedula(cedula) != nullptr;
    }

    Cliente* buscarCliente(int id) {
        int idx = buscarIndice(id);
        return (idx != -1) ? clientes[idx] : nullptr;
    }

    void listarClientes() const {
        cout << "\n----- CLIENTES (" << clientes.size() << ") -----\n";
        for (const auto& c : clientes) { c->mostrarInfo(); cout << " ----\n"; }
    }

    bool actualizarCliente(int id, const string& nuevaDir, const string& nuevoTel) {
        int idx = buscarIndice(id);
        if (idx == -1) return false;
        clientes[idx]->setDireccion(nuevaDir);
       
        cout << " Cliente ID " << id << " actualizado\n";
        return true;
    }

    bool eliminarCliente(int id) {
        int idx = buscarIndice(id);
        if (idx == -1) return false;
        cout << " Cliente '" << clientes[idx]->getNombre() << "' eliminado\n";
        delete clientes[idx];
        clientes.erase(clientes.begin() + idx);
        return true;
    }

    int getTotalClientes() const { return (int)clientes.size(); }

    // Getter solicitado: expone la colección de clientes para reportes simples
    const vector<Cliente*>& getClientes() const { return clientes; }
};

//  SISTEMA CENTRAL – TIENDA

class Tienda {
private:
    string         nombre;
    Inventario     inventario;
    GestorClientes gestorClientes;
    vector<Pedido*>        pedidos;
    vector<Pago*>          pagos;
    vector<Administrador*> admins;
    vector<Vendedor*>      vendedores;

public:
    explicit Tienda(const string& n) : nombre(n) {}

    ~Tienda() {
        for (auto p : pedidos)    delete p;
        for (auto p : pagos)      delete p;
        for (auto a : admins)     delete a;
        for (auto v : vendedores) delete v;
    }

    Inventario&     getInventario()     { return inventario; }
    GestorClientes& getGestorClientes() { return gestorClientes; }
    string          getNombre()   const { return nombre; }

    // Registro de usuarios
    Administrador* registrarAdmin(const string& ced, const string& n, const string& e, const string& t, const string& niv) {
        Administrador* a = new Administrador(ced, n, e, t, niv);
        admins.push_back(a);
        return a;
    }

    Vendedor* registrarVendedor(const string& ced, const string& n, const string& e, const string& t, double porc = 5.0) {
        Vendedor* v = new Vendedor(ced, n, e, t, porc);
        vendedores.push_back(v);
        cout << "  Vendedor '" << n << "' registrado (ID " << v->getId() << ").\n";
        return v;
    }

    // Busqueda polimórfica
    Usuario* buscarPorCedula(const string& ced, string& tipoOut) {
        for (auto a : admins)
            if (a->getCedula() == ced) { tipoOut = "Administrador"; return a; }
        for (auto v : vendedores)
            if (v->getCedula() == ced) { tipoOut = "Vendedor"; return v; }
        Cliente* c = gestorClientes.buscarClientePorCedula(ced);
        if (c) { tipoOut = "Cliente"; return c; }
        return nullptr;
    }

    bool cedulaRegistrada(const string& ced) {
        string tipo;
        return buscarPorCedula(ced, tipo) != nullptr;
    }

    // Pedidos
    Pedido* crearPedido(CarritoDeCompras& carrito, const string& fecha) {
        if (carrito.estaVacio()) { cout << "  El carrito esta vacio.\n"; return nullptr; }

        Pedido* pedido = new Pedido(carrito.getCliente(), fecha);
        const auto& items = carrito.getItems();
        const auto& cants = carrito.getCantidades();

        for (int i = 0; i < (int)items.size(); i++) {
            if (!items[i]->reducirStock(cants[i])) {
                cout << "  Stock insuficiente: '" << items[i]->getNombre() << "'.\n";
                delete pedido;
                return nullptr;
            }
            pedido->agregarLinea(items[i]->getId(), items[i]->getNombre(), items[i]->getPrecio(), cants[i]);
        }

        carrito.getCliente()->agregarPedido(pedido->getId());
        pedidos.push_back(pedido);
        carrito.vaciar();

        cout.setf(ios::fixed); cout.precision(2);
        cout << "  Pedido #" << pedido->getId() << " creado. Total: $" << pedido->getTotal() << "\n";
        return pedido;
    }

    bool procesarPago(Pedido* pedido, Pago* pago) {
        if (!pedido || !pago) return false;
        cout << "\n  ---- Procesando pago para Pedido #" << pedido->getId() << " ----\n";
        pago->mostrarDetalle();
        if (pago->procesar()) {
            pago->generarRecibo();
            pedido->cambiarEstado(Pedido::Estado::CONFIRMADO);
            pagos.push_back(pago);
            return true;
        }
        return false;
    }

    void listarPedidos() const {
        cout << "\n  ---- PEDIDOS (" << pedidos.size() << ") ----\n";
        if (pedidos.empty()) { cout << "  (sin pedidos)\n"; return; }
        for (const auto& p : pedidos) p->mostrar();
    }

    void listarVendedores() const {
        cout << "\n  ---- VENDEDORES (" << vendedores.size() << ") ----\n";
        if (vendedores.empty()) { cout << "  (sin vendedores)\n"; return; }
        for (auto v : vendedores) {
            v->mostrarInfo();
            v->mostrarComisiones();
            cout << "  --\n";
        }
    }

    void mostrarResumen() const {
        cout << "\n  ---- RESUMEN: " << nombre << " ----\n"
             << "  Productos en inventario : " << inventario.getTotalProductos() << "\n"
             << "  Clientes registrados    : " << gestorClientes.getTotalClientes() << "\n"
             << "  Pedidos realizados      : " << pedidos.size() << "\n"
             << "  Pagos procesados        : " << pagos.size() << "\n";
    }

    void generarReporteVentas() const {
        cout << "\n  ---- REPORTE DE VENTAS ----\n";
        if (pedidos.empty()) { cout << "  No hay pedidos registrados.\n"; return; }

        double totalVentas = 0.0;
        for (auto p : pedidos) totalVentas += p->getTotal();

        const vector<Cliente*>& clientes = gestorClientes.getClientes();
        const Cliente* clienteMasActivo = nullptr;
        int maxPedidos = 0;
        for (const auto& c : clientes) {
            int cant = (int)c->getHistorial().size();
            if (cant > maxPedidos) { maxPedidos = cant; clienteMasActivo = c; }
        }

        cout.setf(ios::fixed); cout.precision(2);
        cout << "  Total ingresos     : $" << totalVentas << "\n"
             << "  Pedidos procesados : " << pedidos.size() << "\n";
        if (clienteMasActivo) {
            cout << "  Cliente mas activo : " << clienteMasActivo->getNombre()
                 << " (ID " << clienteMasActivo->getId() << ") con " << maxPedidos << " pedidos\n";
        }
    }
};

// FLUJOS REUTILIZABLES

Pago* solicitarPago(double total) {
    cout.setf(ios::fixed); cout.precision(2);
    cout << "\n  ---- METODO DE PAGO (Total: $" << total << ") ----\n"
         << "  1. Tarjeta\n  2. Efectivo\n  3. Pago digital\n  Opcion: ";
    int op = leerOpcion();

    if (op == 1) {
        string dig = leerTexto("  Ultimos 4 digitos de la tarjeta: ");
        string tit = leerTexto("  Nombre del titular: ");
        string tip = leerTexto("  Tipo (VISA / MASTERCARD): ");
        return new PagoTarjeta(total, fechaHoy(), dig, tit, tip);
    } else if (op == 2) {
        double pagado;
        cout << "  Monto entregado: $";
        cin >> pagado; cin.ignore(1000, '\n');
        try {
            return new PagoEfectivo(total, fechaHoy(), pagado);
        } catch (const invalid_argument& ex) {
            cout << "  Error: " << ex.what() << "\n";
            return nullptr;
        }
    } else if (op == 3) {
        string plat = leerTexto("  Plataforma (PayPal / Nequi / etc.): ");
        string cta  = leerTexto("  Cuenta o numero: ");
        return new PagoDigital(total, fechaHoy(), plat, cta);
    }
    cout << "  Opcion no valida. Pago cancelado.\n";
    return nullptr;
}

bool flujoAgregarProductos(CarritoDeCompras& carrito, Inventario& inv) {
    bool seguir = true;
    while (seguir) {
        inv.listarProductos();
        cout << "\n  ID del producto a agregar (0 para terminar): ";
        int pid = leerOpcion();
        if (pid == 0) break;

        Producto* p = inv.buscarProducto(pid);
        if (!p) { cout << "  Producto no encontrado.\n"; continue; }

        cout << "  Cantidad: ";
        int cant = leerOpcion();
        carrito.agregarItem(p, cant);
        carrito.mostrarCarrito();

        cout << "  Agregar otro producto? (1-Si / 0-No): ";
        seguir = (leerOpcion() == 1);
    }
    return !carrito.estaVacio();
}

// MENUS POR ROL

void menuCliente(Cliente* cliente, Tienda& tienda) {
    CarritoDeCompras carrito(cliente);
    int op;
    do {
        cout << "\n  ----------------------------------------------\n"
             << "  MENU CLIENTE - " << cliente->getNombre() << "\n"
             << "  ----------------------------------------------\n"
             << "  1. Ver productos disponibles\n"
             << "  2. Agregar producto al carrito\n"
             << "  3. Ver mi carrito\n"
             << "  4. Realizar pedido (checkout)\n"
             << "  0. Cerrar sesion\n"
             << "  Opcion: ";
        op = leerOpcion();

        if (op == 1) {
            tienda.getInventario().listarProductos();

        } else if (op == 2) {
            tienda.getInventario().listarProductos();
            cout << "  ID del producto: ";
            int pid = leerOpcion();
            Producto* p = tienda.getInventario().buscarProducto(pid);
            if (!p) { cout << "  Producto no encontrado.\n"; continue; }
            cout << "  Cantidad: ";
            int cant = leerOpcion();
            carrito.agregarItem(p, cant);

        } else if (op == 3) {
            carrito.mostrarCarrito();

        } else if (op == 4) {
            if (carrito.estaVacio()) {
                cout << "  El carrito esta vacio. Agregue productos primero.\n";
                continue;
            }
            carrito.mostrarCarrito();
            cout << "  Confirmar pedido? (1-Si / 0-No): ";
            if (leerOpcion() != 1) continue;

            Pedido* pedido = tienda.crearPedido(carrito, fechaHoy());
            if (!pedido) continue;

            Pago* pago = solicitarPago(pedido->getTotal());
            if (pago) tienda.procesarPago(pedido, pago);
            else      cout << "  Pago cancelado.\n";

        } else if (op != 0) {
            cout << "  Opcion no valida.\n";
        }

    } while (op != 0);
    cout << "  Sesion cerrada. Hasta pronto, " << cliente->getNombre() << ".\n";
}

void menuVendedor(Vendedor* vendedor, Tienda& tienda) {
    int op;
    do {
        cout << "\n  ----------------------------------------------\n"
             << "  MENU VENDEDOR - " << vendedor->getNombre() << "\n"
             << "  ----------------------------------------------\n"
             << "  1. Ver inventario\n"
             << "  2. Registrar venta (pedido para un cliente)\n"
             << "  3. Ver todos los pedidos\n"
             << "  4. Consultar mis comisiones\n"
             << "  0. Cerrar sesion\n"
             << "  Opcion: ";
        op = leerOpcion();

        if (op == 1) {
            tienda.getInventario().listarProductos();

        } else if (op == 2) {
            string ced = leerTexto("  Cedula del cliente: ");
            Cliente* cliente = tienda.getGestorClientes().buscarClientePorCedula(ced);

            if (!cliente) {
                cout << "  Cliente no encontrado.\n"
                     << "  Desea registrarlo ahora? (1-Si / 0-No): ";
                if (leerOpcion() == 1) {
                    if (tienda.cedulaRegistrada(ced)) {
                        cout << "  Esa cedula ya esta registrada en otro perfil.\n";
                        continue;
                    }
                    string nom   = leerTexto("  Nombre completo: ");
                    string email = leerTexto("  Email: ");
                    string tel   = leerTexto("  Telefono: ");
                    string dir   = leerTexto("  Direccion: ");
                    cliente = tienda.getGestorClientes().registrarCliente(ced, nom, email, tel, dir);
                } else {
                    continue;
                }
            }

            cout << "  Cliente: " << cliente->getNombre() << "\n";
            CarritoDeCompras carrito(cliente);

            if (!flujoAgregarProductos(carrito, tienda.getInventario())) {
                cout << "  Venta cancelada (carrito vacio).\n";
                continue;
            }

            carrito.mostrarCarrito();
            cout << "  Confirmar pedido? (1-Si / 0-No): ";
            if (leerOpcion() != 1) continue;

            Pedido* pedido = tienda.crearPedido(carrito, fechaHoy());
            if (!pedido) continue;

            Pago* pago = solicitarPago(pedido->getTotal());
            if (pago) {
                tienda.procesarPago(pedido, pago);
                vendedor->registrarVenta(pedido->getId(), pedido->getTotal());
                cout.setf(ios::fixed); cout.precision(2);
                cout << "  Comision generada por esta venta: $"
                     << pedido->getTotal() * vendedor->getPorcentajeComision() / 100.0 << "\n";
            } else {
                cout << "  Pago cancelado.\n";
            }

        } else if (op == 3) {
            tienda.listarPedidos();

        } else if (op == 4) {
            vendedor->mostrarComisiones();

        } else if (op != 0) {
            cout << "  Opcion no valida.\n";
        }

    } while (op != 0);
    cout << "  Sesion cerrada. Hasta pronto, " << vendedor->getNombre() << ".\n";
}

void menuAdmin(Administrador* admin, Tienda& tienda) {
    int op;
    do {
        cout << "\n  ----------------------------------------------\n"
             << "  MENU ADMINISTRADOR - " << admin->getNombre()
             << " (" << admin->getNivel() << ")\n"
             << "  ----------------------------------------------\n"
             << "  1. Ver inventario\n"
             << "  2. Agregar producto al inventario\n"
             << "  3. Ver clientes registrados\n"
             << "  4. Eliminar cliente\n"
             << "  5. Ver todos los pedidos\n"
             << "  6. Resumen de la tienda\n"
             << "  7. Reporte de ventas\n"
             << "  8. Registrar nuevo vendedor\n"
             << "  9. Ver vendedores y comisiones\n"
             << "  0. Cerrar sesion\n"
             << "  Opcion: ";
        op = leerOpcion();

        if (op == 1) {
            tienda.getInventario().listarProductos();
        } else if (op == 2) {
            string nom  = leerTexto("  Nombre del producto: ");
            string desc = leerTexto("  Descripcion: ");
            string cat  = leerTexto("  Categoria: ");
            double precio;
            int    stock;
            cout << "  Precio: $";
            cin >> precio; cin.ignore(1000, '\n');
            cout << "  Stock inicial: ";
            cin >> stock; cin.ignore(1000, '\n');
            tienda.getInventario().agregarProducto(nom, desc, precio, stock, cat);
        } else if (op == 3) {
            tienda.getGestorClientes().listarClientes();
        } else if (op == 4) {
            tienda.getGestorClientes().listarClientes();
            cout << "  ID del cliente a eliminar: ";
            int id = leerOpcion();
            if (!tienda.getGestorClientes().eliminarCliente(id))
                cout << "  Cliente no encontrado.\n";
        } else if (op == 5) {
            tienda.listarPedidos();
        } else if (op == 6) {
            tienda.mostrarResumen();
        } else if (op == 7) {
            tienda.generarReporteVentas();
        } else if (op == 8) {
            string ced = leerTexto("  Cedula del nuevo vendedor: ");
            if (tienda.cedulaRegistrada(ced)) {
                cout << "  Esa cedula ya esta registrada en el sistema.\n";
                continue;
            }
            string nom   = leerTexto("  Nombre: ");
            string email = leerTexto("  Email: ");
            string tel   = leerTexto("  Telefono: ");
            double porc;
            cout << "  Porcentaje de comision (ej. 5.0): ";
            cin >> porc; cin.ignore(1000, '\n');
            tienda.registrarVendedor(ced, nom, email, tel, porc);
        } else if (op == 9) {
            tienda.listarVendedores();
        } else if (op != 0) {
            cout << "  Opcion no valida.\n";
        }

    } while (op != 0);
    cout << "  Sesion cerrada. Hasta pronto, " << admin->getNombre() << ".\n";
}

void menuPrincipal(Tienda& tienda) {
    int op;
    do {
        cout << "\n  ----------------------------------------------\n"
             << "  AGROFRESH ECUADOR - Sistema de Gestion\n"
             << "  ----------------------------------------------\n"
             << "  1. Ingresar con cedula\n"
             << "  2. Registrarse como cliente\n"
             << "  0. Salir\n"
             << "  Opcion: ";
        op = leerOpcion();

        if (op == 1) {
            string ced = leerTexto("  Ingrese su cedula: ");
            string tipo;
            Usuario* u = tienda.buscarPorCedula(ced, tipo);

            if (!u) {
                cout << "  Cedula no encontrada.\n"
                     << "  Si no tiene cuenta, seleccione la opcion 2 para registrarse.\n";
                continue;
            }

            cout << "  Bienvenido/a, " << u->getNombre() << " - " << tipo << "\n";

            if      (tipo == "Administrador") menuAdmin   (static_cast<Administrador*>(u), tienda);
            else if (tipo == "Vendedor")      menuVendedor(static_cast<Vendedor*>(u),      tienda);
            else if (tipo == "Cliente")       menuCliente (static_cast<Cliente*>(u),       tienda);

        } else if (op == 2) {
            string ced = leerTexto("  Ingrese su cedula: ");
            if (tienda.cedulaRegistrada(ced)) {
                cout << "  Esa cedula ya se encuentra registrada.\n"
                     << "  Por favor ingrese con la opcion 1.\n";
                continue;
            }
            string nom   = leerTexto("  Nombre completo: ");
            string email = leerTexto("  Email: ");
            string tel   = leerTexto("  Telefono: ");
            string dir   = leerTexto("  Direccion: ");
            tienda.getGestorClientes().registrarCliente(ced, nom, email, tel, dir);
            cout << "  Registro exitoso. Ya puede ingresar con su cedula.\n";

        } else if (op != 0) {
            cout << "  Opcion no valida.\n";
        }

    } while (op != 0);

    cout << "\n  Gracias por usar AgroFresh Ecuador. Hasta pronto.\n";
}

// MAIN y Datos Iniciales

int main() {
    Tienda tienda("AgroFresh Ecuador");

    // Datos iniciales cargados en memoria
    tienda.registrarAdmin("1700000001", "Carlos Andrade", "carlos@agrofresh.ec", "0987654321", "GERENTE");
    tienda.registrarVendedor("1700000002", "Maria Lopez", "maria@agrofresh.ec", "0991111222", 5.0);
    
    tienda.getGestorClientes().registrarCliente("1700000003", "Ana Martinez", "ana@gmail.com", "0991234567", "Quito Norte");
    tienda.getGestorClientes().registrarCliente("1700000004", "Luis Garcia", "luis@gmail.com", "0992345678", "Guayaquil");

    tienda.getInventario().agregarProducto("Manzanas Premium", "Manzanas rojas seleccionadas - 10kg", 24.99, 50, "Frutas");
    tienda.getInventario().agregarProducto("Bolsa de Quinua Organica", "Quinua blanca 1kg", 5.99, 100, "Granos");
    tienda.getInventario().agregarProducto("Bananos", "Bananos frescos de la Costa - 20kg", 12.50, 30, "Frutas");

    cout << "\n  Cedulas de prueba:\n"
         << "  - Administrador : 1700000001\n"
         << "  - Vendedor      : 1700000002\n"
         << "  - Cliente Ana   : 1700000003\n"
         << "  - Cliente Luis  : 1700000004\n";

    // Arrancamos el sistema interactivo
    menuPrincipal(tienda);

    return 0;
}
