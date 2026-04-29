#include <iostream>
#include <string>
#include <vector>

using namespace std;

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

    string getTipo() const override { return "Cliente"; } // NUEVO

    void mostrarInfo() const override {
        cout << "  Cliente -> ID: " << id << " | Cedula: " << cedula 
             << " | Nombre: " << nombre << " | Email: " << email
             << " | Tel: " << telefono << " | Dir: " << direccion << "\n";
    }
};

// Administrador 

class Administrador : public Empleado {
private:
    string nivel;
public:
    // Fíjate que ahora llamamos a Empleado y le pasamos "Administrador" como su cargo fijo
    Administrador(const string& ced, const string& n, const string& e,
                  const string& t, const string& niv)
        : Empleado(ced, n, e, t, "Administrador"), nivel(niv) {}

    string getNivel() const { return nivel; }
    string getTipo()  const override { return "Administrador"; }

    void mostrarInfo() const override {
        cout << "  Admin    - ID: " << id
             << " | Cedula: " << cedula
             << " | Nombre: " << nombre
             << " | Email: " << email
             << " | Cargo: " << cargo  // <-- Ahora podemos usar la variable 'cargo' de Empleado
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
    string nombre;
    Inventario inventario;
    GestorClientes gestorClientes;
    vector<Pedido*> pedidos;
    vector<Pago*> pagos;
    vector<Administrador*> admins;
    Administrador* adminActivo;

public:
    explicit Tienda(const string& n) : nombre(n), adminActivo(nullptr) {
        cout << " TechStore '" << nombre << "' iniciada\n";
    }

    ~Tienda() {
        for (auto p : pedidos) delete p;
        for (auto pay : pagos) delete pay;
        for (auto a : admins) delete a;
    }

    Administrador* registrarAdmin(const string& ced, const string& nombre, const string& email, const string& tel, const string& nivel) {
        Administrador* a = new Administrador(ced, nombre, email, tel, nivel);
        admins.push_back(a);
        return a;
    }
    void setAdminActivo(Administrador* a) { adminActivo = a; }

    Inventario& getInventario() { return inventario; }
    GestorClientes& getGestorClientes() { return gestorClientes; }

    // Crear pedido: además de crear el pedido, actualiza historial del cliente
    Pedido* crearPedido(CarritoDeCompras& carrito, const string& fecha) {
        if (carrito.estaVacio()) { cout << " Carrito vacio\n"; return nullptr; }

        Pedido* pedido = new Pedido(carrito.getCliente(), fecha);
        const auto& items = carrito.getItems();
        const auto& cants = carrito.getCantidades();

        for (int i = 0; i < (int)items.size(); i++) {
            if (!items[i]->reducirStock(cants[i])) {
                cout << " Stock insuficiente: '" << items[i]->getNombre() << "'\n";
                delete pedido;
                return nullptr;
            }
            pedido->agregarLinea(items[i]->getId(), items[i]->getNombre(),
                                 items[i]->getPrecio(), cants[i]);
        }

        // registrar pedido en cliente
        carrito.getCliente()->agregarPedido(pedido->getId());
        pedidos.push_back(pedido);
        carrito.vaciar();

        if (adminActivo)
            adminActivo->gestionar("Pedido #" + to_string(pedido->getId()) + " creado");

        cout.setf(ios::fixed);
        cout.precision(2);
        cout << " Pedido #" << pedido->getId()
             << " confirmado  Total: $" << pedido->getTotal() << "\n";
        return pedido;
    }

    bool procesarPago(Pedido* pedido, Pago* pago) {
        if (!pedido || !pago) return false;
        cout << "\n----- Procesando pago para Pedido #" << pedido->getId() << " -----\n";
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
        cout << "\n----- PEDIDOS (" << pedidos.size() << ") -----\n";
        for (const auto& p : pedidos) p->mostrar();
    }

    void mostrarResumen() const {
        cout << "\n___________________\n"
             << " | RESUMEN TIENDA: " << nombre << "\n"
             << " | Productos en inventario : " << inventario.getTotalProductos() << "\n"
             << " | Clientes registrados    : " << gestorClientes.getTotalClientes() << "\n"
             << " | Pedidos realizados      : " << pedidos.size() << "\n"
             << " | Pagos procesados        : " << pagos.size() << "\n\n";
    }

    // Reporte simplificado: total de ingresos y cliente más activo (usa historial de clientes)
    void generarReporteVentas() const {
        cout << "\n=== REPORTE DE VENTAS ===\n";
        if (pedidos.empty()) {
            cout << " No hay pedidos registrados.\n";
            return;
        }

        double totalVentas = 0.0;
        for (auto p : pedidos) totalVentas += p->getTotal();

        // Buscar cliente más activo recorriendo la lista de clientes
        const vector<Cliente*>& clientes = gestorClientes.getClientes();
        const Cliente* clienteMasActivo = nullptr;
        int maxPedidos = 0;
        for (const auto& c : clientes) {
            int cantidad = (int)c->getHistorial().size();
            if (cantidad > maxPedidos) {
                maxPedidos = cantidad;
                clienteMasActivo = c;
            }
        }

        cout.setf(ios::fixed);
        cout.precision(2);
        cout << " Total ingresos: $" << totalVentas << "\n";
        cout << " Pedidos procesados: " << pedidos.size() << "\n";
        if (clienteMasActivo) {
            cout << " Cliente más activo: " << clienteMasActivo->getNombre()
                 << " (ID " << clienteMasActivo->getId() << ") con " << maxPedidos << " pedidos\n";
        } else {
            cout << " No se pudo determinar cliente más activo.\n";
        }
    }
};

// DEMOSTRACION

int main() {
    cout << "|  AGROFRESH ECUADOR - SISTEMA DE GESTION DE VENTAS |\n";

    Tienda tienda("AgroFresh Ecuador");

    Administrador* admin = tienda.registrarAdmin("1700000001", "Carlos Andrade", "carlos@agrofresh.ec", "0987654321", "GERENTE");
    tienda.setAdminActivo(admin);

    GestorClientes& gc = tienda.getGestorClientes();
    
    Cliente* c1 = gc.registrarCliente("1700000003", "Ana Martinez", "ana@gmail.com", "0991234567", "Quito Norte");
    Cliente* c2 = gc.registrarCliente("1700000004", "Luis Garcia", "luis@gmail.com", "0992345678", "Guayaquil");
