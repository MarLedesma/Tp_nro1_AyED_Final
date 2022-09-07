#include <iostream>
#include <fstream>
using namespace std;

const int cod_prod{5};

//punto 1
void kg_list(string nom_prod[],string nom_cliente[],
             float reg_kg [][cod_prod],int cont[],
             int cod_cliente,int cod_prod)
{
    cout << "- punto 1 -" << endl;
    cout << "los elementos que superan los 13000kg son..." << endl;

    for(int i{0};i < cod_cliente;i++){
            //mostrando nombre de cliente...
            cout << i << ". " << nom_cliente[i] <<'\t';

            //Un "if" para emprolijar la matriz
            if(i < 6) cout << '\t';

        for(int j{0};j < cod_prod; j++){

            if(reg_kg [i][j] > 13000){

                //nombrando productos con mas de 13mil Kg...
                cout << nom_prod[j] <<'\t';

                //cont.: cuenta que cliente tiene mas variedad.
                cont[i]+= 1;
            }
        }
        cout << endl;
    }
}
int pos_mayor(int cont_variedad_prod[],int cod_cliente)
{
    int temp = cont_variedad_prod[0];
    int mayor{0};//posicion del cliente con mas variedad

    for(int i{0};i < cod_cliente;i++){
        if(cont_variedad_prod[i] > temp){
            temp = cont_variedad_prod[i];
            mayor = i;
        }
    }
    return mayor;
}
void list_cliente_variedad(string nom_prod[],string nom_cliente[],int cod_cliente,
                           int cont_variedad_prod [],int seg_nom_prod[],float km[][cod_prod])
{
    cout << endl << "- punto 2 -" << endl;
    //pos.:guarda al cliente con mas variedad de productos.
    int pos =pos_mayor(cont_variedad_prod, cod_cliente);

    cout << "variedad de " << nom_cliente[pos] << " posicion "
         << pos << "..." << endl;

    //ordenando segun criterio
    int n,temp{0};
    do{
        n =0;
        for(int i{1};i < cod_prod; i++){
            if(km[pos][i-1] > km[pos][i]){

                temp =km[pos][i-1];
                km[pos][i-1] =km[pos][i];
                km[pos][i] =temp;

                n++;

                temp = seg_nom_prod[i-1];
                seg_nom_prod [i-1] = seg_nom_prod[i];
                seg_nom_prod [i] = temp;
            }
        }
    }
    while(n!=0);

    //Muestra los productos y los Km recorridos en pantalla.
    for(int i{0};i < cod_prod; i++){
        cout << nom_prod [seg_nom_prod [i]] << '\t' << km[pos][i] << "km"<<endl;
    }

}
void mayor_cant_entrega(string nom_prod[],string nom_cliente[],
                        int cant_entregasXprod [][cod_prod], float cant_km[][cod_prod],
                        int cod_cliente,int seg_nom_prod[],int cont_variedad_prod[])
{
    int temp_cant_entregas{0};
    for(int i{0};i < cod_cliente;i++){

        if(i == pos_mayor(cont_variedad_prod,cod_cliente)){

            temp_cant_entregas += cant_entregasXprod[i][0];

        }
        else{
            //cout << i << ". " << nom_cliente[i] << '\t' << cant_entregasXprod[i][seg_nom_prod[0]]<< endl;
            temp_cant_entregas += cant_entregasXprod[i][seg_nom_prod[4]];
        }
    }

    cout << endl << "- punto 3 -" << endl;
    cout << "Cantidad de entregas para el tipo de producto " << nom_prod[seg_nom_prod[4]]
         << ": "<< temp_cant_entregas << endl;
}

int main()
{
    const int cod_cliente{8};
    string nom_cliente[cod_cliente];
    string nom_prod[cod_prod];

    //Suma el peso total de los viajes segun cliente y tipo de producto.
    float Total_pesoXcliente[cod_cliente][cod_prod]{0};

    //cuenta que cliente tiene mas variedad de productos superior a 13mil Kg
    int cont_variedad_prod[cod_cliente]{0};//cont_cliente

    //lo uso para no tener que modificar el orden de nom_prod.
    int re_orden_nom_prod[cod_prod]{0,1,2,3,4};
    float cant_km[cod_cliente][cod_prod]{0};//cant_km

    //indica cuantas entregas tiene cada producto.
    int cant_entregasXprod[cod_cliente][cod_prod]{0};//cant_entregas



    //Cargando nombres de productos y clientes----------------------------
    ifstream reg_nombre;//registro de nombres
    reg_nombre.open("Nombres.txt");

    if(!reg_nombre){
        cout << "error al intentar abrir el archivo de nombre.";
        return 1;
    }

    //carga los nombres de los clientes en ---> string nom_cliente
    for(int i{0};i < cod_cliente;i++){
        reg_nombre >> nom_cliente[i];
    }

    //carga los nombres de los productos en ---> string nom_prod
    for(int i{0};i < cod_prod;i++){
        reg_nombre >> nom_prod[i];
    }
    reg_nombre.close();



    //Carga del registro de los viajes a sus respectivas variables---------------------
    ifstream reg_viajes;
    reg_viajes.open("Datos.txt");

    if(!reg_viajes){
        cout << "error al intentar abrir el archivo de Datos";
        return 2;
    }

    //variables temporales de cod_cliente/ cod_prod/ Peso(enKg)/ Distancia(enKm)
    int tcliente, tprod{0};
    float tkg, tkm{0};

    while(reg_viajes >> tcliente >> tprod >> tkg >> tkm){

        cant_entregasXprod [tcliente][tprod]+= 1;

        Total_pesoXcliente [tcliente][tprod]+= tkg;

        cant_km[tcliente][tprod]+= tkm;
    }
    reg_viajes.close();

    //llamando funciones--------------------------------------------------------------------
    kg_list(nom_prod,nom_cliente,
            Total_pesoXcliente,cont_variedad_prod,
            cod_cliente,cod_prod);

    list_cliente_variedad(nom_prod,nom_cliente,
                          cod_cliente,cont_variedad_prod,
                          re_orden_nom_prod,cant_km);

    mayor_cant_entrega(nom_prod,nom_cliente,
                       cant_entregasXprod,cant_km,
                       cod_cliente,re_orden_nom_prod,cont_variedad_prod);
    return 0;
}
/*
1.
    Emita un listado donde para cada cliente liste los tipos de producto tal que la cantidad total de
    kilos transportados es mayor a 13000.
2.
    Para el cliente con mayor cantidad tipos de productos que hayan superado 13000 kg en el punto
    anterior, emita un listado con nombre de tipo de producto y total de kilómetros recorridos,
    ordenado por total de kilómetros en forma ascendente.
    Nota: No alterar el vector de nombres.
3.
    Del listado anterior, tome el tipo de producto con mayor cantidad de kilómetros recorridos e
    informe cuantas entregas se hicieron para ese tipo de producto (sin importar para que cliente).
*/
