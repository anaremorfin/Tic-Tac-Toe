/**
*     @file Gato.c
*
*     @brief Este programa consiste en un Juego de Gato con interfaz grafica GTK.
*     Puede jugarse de dos personas o jugador contra el programa. Tiene la finalidad
*     de divertir al jugador y retarlo, asi como interactuar con una buena interfaz
*     grafica.
*
*     @author Juan Castillo Carrillo, Ana Regina Morfin Marquez, y David Emmanuel Perez Avila
*     @date 13/05/2020
*/

#include <gtk/gtk.h>
#include "gato.h"
#include "nombres.h"

typedef struct parametros
{
int modo;
int turno;
char nombre1[30];
char nombre2[30];
int tiros[9];
char tablero[10];
int vf;
int ti;
int ban;
GtkWidget *botones[9];
GtkWidget *ventana;
GtkWidget *entry1, *entry2;
GtkWidget *labelmodo, *labelturno;
}nodo;

void Texto(GtkWidget *button, gpointer data);
void Boton(GtkButton *button, gpointer data);
void reinicio(GtkButton *p, gpointer data);
void lee(GtkToggleButton *button, gpointer data);
void guarda(GtkToggleButton *button, gpointer data);
void leear(char nombre[], nodo *datos);
void guardaar(char nombre[], nodo *datos);
void ventana2(GtkWindow *parent, gpointer data);
void ventana3(GtkWindow *parent, gpointer data);
void tianterior(GtkWidget *boton, gpointer data);
void tinext(GtkWidget *boton, gpointer data);
void destruir(GtkWidget *d,gpointer data)
{
gtk_widget_destroy(d);
}
void destroy(GtkWidget *widget, gpointer data)
{
gtk_main_quit();
}

int main(int argc, char *argv[])
{

GtkWidget *window;
GtkWidget *box;
GtkWidget *button;
GtkWidget *image;
GtkWidget *xbox2;
GtkWidget *xbox;
GtkWidget *barra, *raiz, *menu, *op1, *op2;
GtkWidget *ayuda, *acerca;
GtkWidget *turno, *estado, *both;
GtkWidget *adelante, *atras, *adelanteyatras;
//GtkWidget *cajabarra;
GdkColor color;
//Variables del programa
nodo lista;
//PA los botones
int i;
float a,x,y=0;
char fra[25];

//Inicializar
lista.modo=0;
lista.turno=1;
for(i=0;i<9;i++)
{
lista.tablero[i]='I';
lista.tiros[i]=-2;
}
lista.vf=3;
lista.ti=-1;
lista.tablero[9]='\0';
lista.ban=0;

gtk_init(&argc, &argv);

//Ventana con  titulo, size fijo
window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window),"TIC-TAC-TOE");
gtk_signal_connect(GTK_OBJECT(window),"destroy", GTK_SIGNAL_FUNC(destroy), NULL);
gtk_container_set_border_width(GTK_CONTAINER(window),0);
gtk_window_set_resizable(GTK_WINDOW(window), FALSE); //No se puede  cambiar el tamano
gtk_window_set_default_size(GTK_WINDOW(window), 300,300);

//Caja para guardar
xbox2 = gtk_vbox_new(FALSE,5); //botones de gato
xbox=gtk_vbox_new(FALSE,25); //contiene a las  demas cajas
box=gtk_hbox_new(TRUE,0); //botones individuales
both=gtk_hbox_new(TRUE,0); //aqui se guarda turno y estado
adelanteyatras=gtk_hbox_new(TRUE,0); //botones flechas
//cajabarra=gtk_vbox_new(TRUE,0);

//Menu de Barra

barra = gtk_menu_bar_new(); //barra de menu superior
gdk_color_parse("#f27976", &color); //color rosa
gtk_widget_modify_bg(GTK_WIDGET(barra),GTK_STATE_NORMAL, &color);

//Archivo
menu = gtk_menu_new(); //elemento de la barra
op1 = gtk_menu_item_new_with_label("Cargar"); //al desplegarse manu
op2 = gtk_menu_item_new_with_label("Guardar");
gdk_color_parse("#f1b7a3", &color);
gtk_widget_modify_bg(GTK_WIDGET(menu),GTK_STATE_NORMAL, &color);

gtk_signal_connect(GTK_OBJECT(op1),"activate",GTK_SIGNAL_FUNC(lee),&lista); //llama a funcion de leer archivo
gtk_signal_connect(GTK_OBJECT(op2),"activate",GTK_SIGNAL_FUNC(guarda),&lista); //llama a funcion de guardar archivo

gtk_menu_append(GTK_MENU(menu),op1); //insertar las opciones al menu de archivo
gtk_menu_append(GTK_MENU(menu),op2);

raiz = gtk_menu_item_new_with_label("Archivo");

gtk_menu_item_set_submenu(GTK_MENU_ITEM(raiz),menu);

gtk_menu_bar_append(GTK_MENU_BAR(barra),raiz);

//Juego
menu = gtk_menu_new();
op1 = gtk_menu_item_new_with_label("J VS. J"); //jugador vs. jugador
op2 = gtk_menu_item_new_with_label("J VS. CPU"); //jugador vs. CPU

gtk_signal_connect(GTK_OBJECT(op1),"activate",GTK_SIGNAL_FUNC(ventana2), &lista);
gtk_signal_connect(GTK_OBJECT(op2),"activate",GTK_SIGNAL_FUNC(ventana3), &lista);

gtk_widget_modify_bg(GTK_WIDGET(menu),GTK_STATE_NORMAL, &color);
gdk_color_parse("#f1b7a3", &color);
gtk_widget_modify_bg(GTK_WIDGET(op1),GTK_STATE_ACTIVE, &color);

gtk_menu_append(GTK_MENU(menu),op1);
gtk_menu_append(GTK_MENU(menu),op2);

raiz = gtk_menu_item_new_with_label("Juego");

gtk_menu_item_set_submenu(GTK_MENU_ITEM(raiz),menu);

gtk_menu_bar_append(GTK_MENU_BAR(barra),raiz);

//Ayuda
menu = gtk_menu_new();
raiz = gtk_menu_item_new_with_label("Ayuda");
ayuda = gtk_menu_item_new_with_label("\nEl objetivo es hacer una linea con la misma figura de forma horizontal, vertical o diagonal.\nSe debe evitar que el oponente complete la linea primero.\n................................................................................................................................................................................................\nPara cargar un archivo: Archivo//Cargar.\nSeleccione el archivo deseado y presione Open.\nSi desea regresar a la ventana principal sin abrir ningun archivo, presione Cancel.\n................................................................................................................................................................................................\nPara guardar un archivo: Archivo//Guardar.\nIntroduzca el nombre del archivo y donde desea que se guarde; presione Save.\n................................................................................................................................................................................................\nExisten dos opciones: jugador contra jugador, o jugador contra el programa.\nSi desea jugar contra otro jugador, presione J. vs. J.\nUna vez seleccionado, escriba los nombres de los jugadores en la ventana emergente y presione Aceptar.\nColoque su tiro sobre el tablero y comienza el juego.\nLos turnos entre los jugadores se van alternando.\nCuando hay un ganador, ya no se puede tirar y se muestra en la terminal quien gano.\n................................................................................................................................................................................................\nPara iniciar una nueva partida, coloquese sobre Juego y seleccione una opcion.\nPara jugar contra el programa coloquese sobre Juego y seleccione J vs. CPU.\nDespues, escriba su nombre en la ventana emergente y presione Aceptar.\nPresione la casilla sobre el tablero en la que desea colocar su X y el programa respondera como su oponente O.\n................................................................................................................................................................................................\nAl finalizar se muestra el ganador en la terminal y ya no se permite tirar mas.\nCierre el Juego de Gato presionando X en la esquina superior derecha de la ventana.\n................................................................................................................................................................................................\nLa flecha izquierda en la parte inferior sirve para regresar a la jugada y turno anterior.\nSe puede utilizar la flecha derecha en la parte inferior para volver a la jugada posterior una vez que se haya utilizado\nla flecha de anterior, o bien para navegar sobre el juego cargado de un archivo.\n................................................................................................................................................................................................\n");
gtk_widget_modify_bg(GTK_WIDGET(menu),GTK_STATE_NORMAL, &color);
gdk_color_parse("#f1b7a3", &color);
gtk_menu_append(GTK_MENU(menu),ayuda);
gtk_menu_item_set_submenu(GTK_MENU_ITEM(raiz),menu);

gtk_menu_bar_append(GTK_MENU_BAR(barra),raiz);

//Acerca de
menu = gtk_menu_new();
raiz = gtk_menu_item_new_with_label("Acerca De");
acerca = gtk_menu_item_new_with_label("Programacion Aplicada y Lab.\n..............................................................................................................................\nProyecto Final\nJUEGO DE GATO\n..............................................................................................................................\nPrograma realizado por:\nJuan Castillo Carrillo, Ana Regina Morfin, y David Emmanuel Perez Avila\n..............................................................................................................................\nMayo2020\n");
gtk_widget_modify_bg(GTK_WIDGET(menu),GTK_STATE_NORMAL, &color);
gdk_color_parse("#f1b7a3", &color);
gtk_menu_append(GTK_MENU(menu),acerca);
gtk_menu_item_set_submenu(GTK_MENU_ITEM(raiz),menu);

//Barra hasta arriba en xbox
gtk_menu_bar_append(GTK_MENU_BAR(barra),raiz);

gtk_box_pack_start(GTK_BOX(xbox),barra,FALSE,FALSE,0);

//Label estado y turno
estado = gtk_label_new("Estado: En Reposo");
gdk_color_parse("#f8b28e", &color);
gtk_widget_modify_fg(GTK_WIDGET(estado),GTK_STATE_ACTIVE, &color);
turno = gtk_label_new("Turno: ");
gdk_color_parse("#4ca4b8", &color);
gtk_widget_modify_fg(GTK_WIDGET(turno),GTK_STATE_ACTIVE, &color);

gtk_box_pack_start(GTK_BOX(both),estado,FALSE,TRUE,0);
gtk_box_pack_start(GTK_BOX(both),turno,FALSE,TRUE,0);

gtk_box_pack_start(GTK_BOX(xbox),both,FALSE,FALSE,0); //incluye estado y turno

lista.labelmodo=estado;
lista.labelturno=turno;

//Botones
for(i=0;i<9;i++) //se crean 9 botones en forma de 3x3
{
a=i;
x=(a/100);
button=gtk_button_new_with_mnemonic(" ");
gdk_color_parse("#dfece2", &color);
gtk_widget_modify_bg(GTK_WIDGET(button),GTK_STATE_NORMAL, &color);
gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(Boton), &lista);
sprintf(fra, "GatoIMG/b%d.jpg", i+1); //imprimir imagen blanca
image=gtk_image_new_from_file(fra);
gtk_button_set_image(GTK_BUTTON(button),image);
gtk_button_set_alignment(GTK_BUTTON(button), x, y);
gtk_box_pack_start(GTK_BOX(box), button, TRUE,TRUE, 2.5);
gtk_widget_show(button);

if(i==2 || i==5|| i==8) //ultima columna
{
gtk_container_add(GTK_CONTAINER(xbox2),box);
box=gtk_hbox_new(TRUE,0);
}
lista.botones[i]=button;
}

gtk_box_pack_start(GTK_BOX(xbox),xbox2,FALSE,TRUE,0); //empaco

//Adelante y atras
atras = gtk_button_new();
gdk_color_parse("#4ca4b8", &color);
gtk_widget_modify_bg(GTK_WIDGET(atras),GTK_STATE_NORMAL, &color);
gtk_box_pack_start(GTK_BOX(adelanteyatras),atras,FALSE,TRUE,0); //empaco
sprintf(fra, "GatoIMG/antes.jpg");
image=gtk_image_new_from_file(fra);
gtk_button_set_image(GTK_BUTTON(atras),image);
gtk_signal_connect(GTK_OBJECT(atras),"clicked",GTK_SIGNAL_FUNC(tianterior), &lista);

adelante = gtk_button_new();
gdk_color_parse("#b5d1bb", &color);
gtk_widget_modify_bg(GTK_WIDGET(adelante),GTK_STATE_NORMAL, &color);
gtk_box_pack_start(GTK_BOX(adelanteyatras),adelante,FALSE,TRUE,0); //empaco
sprintf(fra, "GatoIMG/despues.jpg");
image=gtk_image_new_from_file(fra);
gtk_button_set_image(GTK_BUTTON(adelante),image);
gtk_signal_connect(GTK_OBJECT(adelante),"clicked",GTK_SIGNAL_FUNC(tinext), &lista);

gtk_box_pack_start(GTK_BOX(xbox),adelanteyatras,FALSE,TRUE,0); //empaco
gtk_widget_show(atras);
gtk_widget_show(adelante);

gtk_container_add(GTK_CONTAINER(xbox),box);
gdk_color_parse("#0b6889", &color);
gtk_widget_modify_bg(GTK_WIDGET(window),GTK_STATE_NORMAL, &color);

//Parte final
gtk_container_add(GTK_CONTAINER(window),xbox);
gtk_widget_show_all(window);

gtk_main();

return(0);

}

/**
*     Esta funcion sirve para conocer en donde el usuario desea colocar su simbolo.
*     En caso de que la casilla deseada este disponible, se coloca el simbolo en
*     la casilla deseada. Se llevan a cabo las cuentas para conocer el estado del juego,
*     es decir, si ya termino o esta en juego.
*
*     @param data   Recibe los datos para llevar a cabo  la funcion.
*/

void Boton(GtkButton *button, gpointer data)
{
  GtkWidget *image;
  nodo *dato;
  dato=(nodo *)data;
  char fra[60];
  int i;
  int j;
  int aux;
  int at=-1;
  float x,y;

  gtk_button_get_alignment(button, &x,&y); //conocer boton seleccionado
  i=(x*100)+1;
  j=i-1;

  if(dato->turno ==10 || dato->vf==2 || dato->vf==1 || dato->modo==0)  //bandera para programa en reposo
  g_print("Ya gano alguien\n");
  else
  {
    if(dato->tablero[j]=='I') //vacio
    {
      if(dato->tiros[dato->turno] >=0 && dato->ban==0)  //bandera de flecha para saber quien selecciono boton
      {
        for(aux=dato->turno;aux<9; aux++)
        dato->tiros[aux]=-1;
      }
      dato->ban=0;
      JVJNP(&dato->turno, j, &dato->vf, dato->tablero); //manda llamar a funcion para tirar de acuerdo a turno
      dato->turno--;
      if(dato->turno==1||dato->turno==3||dato->turno==5||dato->turno==7||dato->turno==9) //Colocar X
      {
        sprintf(fra,"GatoIMG/t%d.gif",i); //poner gif
        image=gtk_image_new_from_file(fra);
        gtk_button_set_image(button, image);
        sprintf(fra,"Turno: %s", dato->nombre2);
        gtk_label_set_text(GTK_LABEL(dato->labelturno), fra);
      }
      if(dato->turno==2||dato->turno==4||dato->turno==6||dato->turno==8) //Colocar O
      {
        sprintf(fra,"GatoIMG/c%d.gif",i);
        image=gtk_image_new_from_file(fra);
        gtk_button_set_image(button, image);
        sprintf(fra,"Turno: %s", dato->nombre1);
        gtk_label_set_text(GTK_LABEL(dato->labelturno), fra);

      }
      dato->tiros[dato->turno -1]=j;
      dato->turno++;

      if(dato->modo==2 && dato->turno!=10 && dato->vf!=1) //Jugador contra CPU
      {
        if(dato->turno==2) //Primer turno de CPU
        {
          dato->ti = tiroinicial(dato->tablero,&dato->turno, &at); //Primer tiro de CPU. De acuerdo a lo que regresa ti, se coloca gif.
          dato->turno--;
          JVJNP(&dato->turno, at, &dato->vf, dato->tablero);
          dato->tiros[dato->turno-2]=at;
          sprintf(fra,"GatoIMG/c%d.gif",at+1);
          image=gtk_image_new_from_file(fra);
          gtk_button_set_image(GTK_BUTTON(dato->botones[at]), image);
          sprintf(fra,"Turno: %s", dato->nombre1);
          gtk_label_set_text(GTK_LABEL(dato->labelturno), fra);

        }
        else //Otros tiros CPU
        {
          at=ataque(dato->tablero, &dato->turno, 'O'); //Completa lineas
          if (at==-1)
          at=ataque(dato->tablero, &dato->turno, 'X'); //Bloquea lineas del oponente
          if(at==-1)
          at=tiroestrategico(dato->tablero, &dato->turno, dato->ti);  //Si no bloque ni ataca, busca tiro optimo
          dato->turno--;
          dato->tiros[dato->turno -1]=at;
          JVJNP(&dato->turno, at, &dato->vf, dato->tablero);
          sprintf(fra,"GatoIMG/c%d.gif",at+1);
          image=gtk_image_new_from_file(fra);
          gtk_button_set_image(GTK_BUTTON(dato->botones[at]), image);
          sprintf(fra,"Turno: %s", dato->nombre1);
          gtk_label_set_text(GTK_LABEL(dato->labelturno), fra);

        }
      }
      if(dato->vf==1) //Ganador es jugador1, X
      {
        sprintf(fra,"Ganador: %s", dato->nombre1);
        gtk_label_set_text(GTK_LABEL(dato->labelmodo), fra);
        gtk_label_set_text(GTK_LABEL(dato->labelturno), "FIN");
      }
      else if(dato->vf==2) //Ganador es jugador2, O, o CPU.
      {
        sprintf(fra,"Ganador: %s", dato->nombre2);
        gtk_label_set_text(GTK_LABEL(dato->labelmodo), fra);
        gtk_label_set_text(GTK_LABEL(dato->labelturno), "FIN");
      }
      else if(dato->turno==10) //Empate
      {
        gtk_label_set_text(GTK_LABEL(dato->labelmodo), "Empate");
        gtk_label_set_text(GTK_LABEL(dato->labelturno), "FIN");
      }
    }
  }
}

/**
*     Esta funcion sirve para reiniciar los datos a su valor
*     original e iniciar nuevo juego.Limpia las casillas del
*     gato colocando la imagen en blanco.
*
*     @param data   Recibe los datos para llevar a cabo  la
*     funcion.
*/

void reinicio(GtkButton *p, gpointer data)
{
  nodo *lista;
  int i;
  const char *opc;
  lista=(nodo *)data;
  char fra[50];
  opc=gtk_button_get_label(p);
  char *nombre1, *nombre2;
  GtkWidget *image;

  if(strcmp(opc,"OK")==0) //bandera para conocer si se selecciono J. vs. J.
  {
    lista->modo=1;
    gtk_label_set_text(GTK_LABEL(lista->labelmodo), "Jugador vs. Jugador");
    nombre1=gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(lista->entry1));
    nombre2=gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(lista->entry2));
    if(strcmp(nombre1,"")==0)
    strcpy(lista->nombre1,"J1");
    else
    {
      strcpy(lista->nombre1,nombre1);
      guar_list(nombre1);

    }
    if(strcmp(nombre2,"")==0)
    strcpy(lista->nombre2,"J2");
    else
    {
      strcpy(lista->nombre2,nombre2);
      guar_list(nombre2);
    }
    sprintf(fra,"Turno: %s", lista->nombre1);
    gtk_label_set_text(GTK_LABEL(lista->labelturno), fra);
  }
  if(strcmp(opc,"0K")==0) //bandera para conocer si se selecciono J. vs. C.P.U
  {
    lista->modo=2;
    gtk_label_set_text(GTK_LABEL(lista->labelmodo), "Jugador vs. C.P.U");
    nombre1=gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(lista->entry1));
    if(strcmp(nombre1,"")==0)
    strcpy(lista->nombre1,"J1");
    else
    {
      strcpy(lista->nombre1,nombre1);
      guar_list(nombre1);

    }
    sprintf(fra,"Turno: %s", lista->nombre1);
    gtk_label_set_text(GTK_LABEL(lista->labelturno), fra);
    strcpy(lista->nombre2,"CPU");
  }
  lista->turno=1;
  for(i=0;i<9;i++)
  {
    lista->tablero[i]='I';
    lista->tiros[i]=-1;
    sprintf(fra, "GatoIMG/b%d.jpg", i+1);
    image=gtk_image_new_from_file(fra);
    gtk_button_set_image(GTK_BUTTON(lista->botones[i]),image);
  }

  lista->vf=0;
  destruir(lista->ventana, NULL); //Widget desde el cual se envia la senal de reinicio se destruye

}

/**
*     Esta funcion sirve abrir un archivo de juego previamente
*     guardado.
*
*     @param data   Recibe los datos para llevar a cabo  la
*     funcion.
*/

void lee(GtkToggleButton *button, gpointer data)
{
  GtkWidget *dialogo;
  gchar *nombre;
  nodo *datos;
  datos=(nodo *)data;

  dialogo = gtk_file_chooser_dialog_new ("Abrir Archivo",
  NULL,
  GTK_FILE_CHOOSER_ACTION_OPEN,
  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
  NULL);

  if (gtk_dialog_run(GTK_DIALOG(dialogo)) == GTK_RESPONSE_ACCEPT)
  {
    nombre = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialogo));
    leear(nombre, datos);
    g_free (nombre);
  }

  gtk_widget_destroy(dialogo);
}

/**
*     Esta funcion sirve guardar un archivo de juego previamente
*     comenzado.
*
*     @param data   Recibe los datos para llevar a cabo  la
*     funcion.
*/
void guarda(GtkToggleButton *button, gpointer data)
{

  GtkWidget *dialogo;
  nodo *datos;
  datos=(nodo *)data;
  gchar *nombre;

  dialogo = gtk_file_chooser_dialog_new ("Guardar Archivo",
  NULL,
  GTK_FILE_CHOOSER_ACTION_SAVE,
  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
  NULL);

  if (gtk_dialog_run(GTK_DIALOG(dialogo)) == GTK_RESPONSE_ACCEPT)
  {
    nombre = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialogo));
    guardaar(nombre, datos);
    g_free (nombre);
  }

  gtk_widget_destroy(dialogo);

}

/**
*     Esta funcion sirve cargar un archivo de juego previamente
*     utilizado. Se puede continuar el juego en caso de que este
*     no haya sido finalizado, o bien, obervar los movimientos
*     jugada a jugada.
*
*     @param data   Recibe los datos para llevar a cabo  la
*     funcion.
*/
void leear(char nombre[], nodo *d)
{
  FILE *archivo;
  GtkWidget *image;
  char linea[150];
  int i;
  archivo=fopen(nombre, "rt");

  fscanf(archivo,"%s\n",linea);
  if(strcmp(linea, "WA21JM10")==0) //codigo de seguridad
  {
    fscanf(archivo,"%d\n",&d->modo);
    if(d->modo==1) //J.vs. J
    gtk_label_set_text(GTK_LABEL(d->labelmodo), "Jugador vs. Jugador");
    if(d->modo==2) //J. vs. CPU
    gtk_label_set_text(GTK_LABEL(d->labelmodo), "Jugador vs. C.P.U");
    fscanf(archivo,"%d\n",&d->turno);
    d->turno=1;
    fscanf(archivo,"%s\n",d->nombre1);
    fscanf(archivo,"%s\n",d->nombre2);
    if((d->turno %2)==1)
    sprintf(linea,"Turno: %s", d->nombre1);
    else
    sprintf(linea,"Turno: %s", d->nombre2);
    gtk_label_set_text(GTK_LABEL(d->labelturno), linea);
    fscanf(archivo,"%s\n",d->tablero);
    for(i=0;i<9;i++)
    {
      fscanf(archivo,"%d\n",&d->tiros[i]);
      d->tablero[i]='I';
      sprintf(linea,"GatoIMG/b%d.jpg",i+1);
      image=gtk_image_new_from_file(linea);
      gtk_button_set_image(GTK_BUTTON(d->botones[i]), image);

    }
    fscanf(archivo,"%d\n",&d->vf);
    d->vf=3;
    fscanf(archivo,"%d\n",&d->ti);
  }
  else
  g_print("Archivo no valido");
  fclose(archivo);

}

/**
*     Esta funcion sirve guardar un archivo de juego previamente
*     comenzado. Se puede guardar el juego durante la partida,
*     o bien, al finalizar. El archivo incluye toda la informacion
*     del juego descrita en la estructura.
*     jugada a jugada.
*
*     @param data   Recibe los datos para llevar a cabo  la
*     funcion.
*/
void guardaar(char nombre[], nodo *d)
{
  FILE *archivo;
  int i;

  archivo=fopen(nombre, "wt");

  fprintf(archivo,"WA21JM10\n"); //codigo de seguridad
  fprintf (archivo,"%d\n",d->modo);
  fprintf (archivo,"%d\n",d->turno);
  fprintf (archivo,"%s\n",d->nombre1);
  fprintf (archivo,"%s\n",d->nombre2);
  fprintf (archivo,"%s\n",d->tablero);
  for(i=0;i<9;i++)
  fprintf (archivo,"%d\n",d->tiros[i]);
  fprintf (archivo,"%d\n",d->vf);
  fprintf (archivo,"%d\n",d->ti);

  fclose(archivo);

}

/**
*     Función utilizado para motivos de prueba
*
*     @param data   Recibe los datos para llevar a cabo  la
*     funcion.
*/
void Texto(GtkWidget *button, gpointer data)
{
  char *nombre;

  nombre=gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(data));

  g_print("%s", nombre);

}

/**
*     Esta funcion crea una ventana para poder pedir
*     y escanear los nombres del jugador. (Oponente CPU)
*
*     @param data   Recibe los datos para llevar a cabo
*     la funcion.
*/
void ventana3(GtkWindow *button, gpointer data)
{

  GtkWidget *window, *label, *boton4;
  GtkWidget *vbox;
  GtkWidget *combo;
  GtkWidget *cajabox;
  GtkWidget *box1;
  GdkColor color;

  nodo *lista=(nodo *)data;

  box1=gtk_hbox_new(TRUE,0);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window),"NP J VS. CPU");
  gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
  gtk_container_border_width(GTK_CONTAINER(window),30);
  gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(destruir),NULL);

  lista->ventana=window;

  vbox = gtk_vbox_new(TRUE, 0);
  gtk_widget_show(vbox);
  cajabox=gtk_vbox_new(TRUE, 0);
  gtk_widget_show(cajabox);

  label = gtk_label_new("JUGADOR 1");
  gdk_color_parse("#268297", &color);
  gtk_widget_modify_bg(GTK_WIDGET(label),GTK_STATE_NORMAL, &color);
  gtk_widget_show(label);
  gtk_widget_show(box1);
  gtk_box_pack_start(GTK_BOX(box1),label,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox),box1,FALSE,FALSE,0);

  combo=gtk_combo_box_text_new_with_entry();
  gtk_widget_show(combo);
  lee_list(combo); //Funcion para mostrar los nombres de jugadores preestablecidos o previamente guardados
  lista->entry1=combo;
  //gdk_color_parse("#f8b28e", &color);
  //gtk_widget_modify_fg(GTK_WIDGET(combo),GTK_STATE_NORMAL, &color);
  gtk_box_pack_start(GTK_BOX(cajabox),combo,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox), cajabox, FALSE, FALSE, 0);

  boton4 = gtk_button_new_with_label("0K");
  gtk_widget_show(boton4);
  gtk_signal_connect(GTK_OBJECT(boton4),"clicked",GTK_SIGNAL_FUNC(reinicio), lista);
  gdk_color_parse("#f37976", &color);
  gtk_widget_modify_bg(GTK_WIDGET(boton4),GTK_STATE_NORMAL, &color);
  gtk_box_pack_start(GTK_BOX(vbox),boton4,FALSE,FALSE,0);

  gtk_container_add(GTK_CONTAINER(window),vbox);
  gdk_color_parse("#b5d1bb", &color);
  gtk_widget_modify_bg(GTK_WIDGET(window),GTK_STATE_NORMAL, &color);

  gtk_widget_show_all(window);
}

/**
*     Esta funcion crea una ventana para poder pedir
*     y escanear los nombres de los jugadores (J. vs. J.)
*
*     @param data   Recibe los datos para llevar a cabo
*     la funcion.
*/
void ventana2(GtkWindow *button, gpointer data)
{
  GtkWidget *window, *label, *label2, *boton4;
  GtkWidget *vbox;
  GtkWidget *combo, *combo2;
  GtkWidget *cajabox, *cajabox2;
  GtkWidget *box1, *box2;
  GdkColor color;
  nodo *lista=(nodo *)data;

  box1=gtk_hbox_new(TRUE,0);
  box2=gtk_hbox_new(TRUE,0);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  lista->ventana=window;
  gtk_window_set_title(GTK_WINDOW(window),"NP J VS. J");
  gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
  gtk_container_border_width(GTK_CONTAINER(window),30);
  gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(destruir),NULL);

  vbox = gtk_vbox_new(TRUE, 0);
  gtk_widget_show(vbox);
  cajabox=gtk_vbox_new(TRUE, 0);
  gtk_widget_show(cajabox);
  cajabox2=gtk_vbox_new(TRUE, 0);
  gtk_widget_show(cajabox2);

  label = gtk_label_new("JUGADOR 1");
  gdk_color_parse("#268297", &color);
  gtk_widget_modify_bg(GTK_WIDGET(label),GTK_STATE_NORMAL, &color);
  gtk_widget_show(label);
  gtk_widget_show(box1);
  gtk_box_pack_start(GTK_BOX(box1),label,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox),box1,FALSE,FALSE,0);

  combo=gtk_combo_box_text_new_with_entry();
  gtk_widget_show(combo);
  lee_list(combo);
  lista->entry1=combo;
  //gdk_color_parse("#f8b28e", &color);
  //gtk_widget_modify_fg(GTK_WIDGET(combo),GTK_STATE_NORMAL, &color);
  gtk_box_pack_start(GTK_BOX(cajabox),combo,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox), cajabox, FALSE, FALSE, 0);

  label2 = gtk_label_new("JUGADOR 2");
  gdk_color_parse("#268297", &color);
  gtk_widget_modify_bg(GTK_WIDGET(label2),GTK_STATE_NORMAL, &color);
  gtk_widget_show(label2);
  gtk_widget_show(box2);
  gtk_box_pack_start(GTK_BOX(box2),label2,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox),box2,FALSE,FALSE,0);

  combo2=gtk_combo_box_text_new_with_entry();
  gtk_widget_show(combo2);
  lee_list(combo2);
  lista->entry2=combo2;
  gtk_box_pack_start(GTK_BOX(cajabox2),combo2,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox), cajabox2, FALSE, FALSE, 0);


  boton4 = gtk_button_new_with_label("OK");
  gtk_widget_show(boton4);
  gtk_signal_connect(GTK_OBJECT(boton4),"clicked",GTK_SIGNAL_FUNC(reinicio), lista);
  gdk_color_parse("#f37976", &color);
  gtk_widget_modify_bg(GTK_WIDGET(boton4),GTK_STATE_NORMAL, &color);
  gtk_box_pack_start(GTK_BOX(vbox),boton4,FALSE,FALSE,0);

  gtk_container_add(GTK_CONTAINER(window),vbox);
  gdk_color_parse("#b5d1bb", &color);
  gtk_widget_modify_bg(GTK_WIDGET(window),GTK_STATE_NORMAL, &color);

  gtk_widget_show_all(window);
}

/**
*     Esta funcion permite regresar al tiro anterior, modificando
*     los datos y restableciendo la casilla a imagen en blanco.
*
*     @param data   Recibe los datos para llevar a cabo
*     la funcion.
*/
void tianterior(GtkWidget *boton, gpointer data)
{
  nodo *lista;
  lista=(nodo *)data;
  GtkWidget *image;
  char fra[50];
  int esp;
  int cas;
  int tcpu;
  int cascpu;
  if(lista->modo==1 && lista->turno!=1) //J. vs. J.
  {
    if(lista->turno==10 || lista->vf!=3)
    {
      gtk_label_set_text(GTK_LABEL(lista->labelmodo), "Jugador vs. Jugador");
      sprintf(fra,"Turno: %s", lista->nombre1);
      gtk_label_set_text(GTK_LABEL(lista->labelturno), fra);
      lista->vf=3;
    }
    lista->turno--;
    esp=(lista->turno -1);
    cas=lista->tiros[esp];
    lista->tablero[cas]='I';
    sprintf(fra, "GatoIMG/b%d.jpg", cas+1);
    image=gtk_image_new_from_file(fra);
    gtk_button_set_image(GTK_BUTTON(lista->botones[cas]),image);
    if(esp%2==1)
    {
      sprintf(fra,"Turno: %s", lista->nombre2);
      gtk_label_set_text(GTK_LABEL(lista->labelturno), fra);
    }
    else
    {
      sprintf(fra,"Turno: %s", lista->nombre1);
      gtk_label_set_text(GTK_LABEL(lista->labelturno), fra);
    }
  }
  if(lista->modo==2 && lista->turno!=1)
  {
    if(lista->turno==10 || lista->vf!=3)
    {
      gtk_label_set_text(GTK_LABEL(lista->labelmodo), "Jugador vs. C.P.U");
      sprintf(fra,"Turno: %s", lista->nombre1);
      gtk_label_set_text(GTK_LABEL(lista->labelturno), fra);
      lista->vf=3;
    }
    if(lista->turno==10)
    {
      lista->turno--;
      esp=(lista->turno -1);
      cas=lista->tiros[esp];
      lista->tablero[cas]='I';
      sprintf(fra, "GatoIMG/b%d.jpg", cas+1);
      image=gtk_image_new_from_file(fra);
      gtk_button_set_image(GTK_BUTTON(lista->botones[cas]),image);
    }
    else
    {
      lista->turno--;
      tcpu=(lista->turno -1);
      esp=(lista->turno -2);
      cascpu=lista->tiros[tcpu];
      cas=lista->tiros[esp];
      lista->tablero[cas]='I';
      lista->tablero[cascpu]='I';
      sprintf(fra, "GatoIMG/b%d.jpg", cas+1);
      image=gtk_image_new_from_file(fra);
      gtk_button_set_image(GTK_BUTTON(lista->botones[cas]),image);
      sprintf(fra, "GatoIMG/b%d.jpg", cascpu+1);
      image=gtk_image_new_from_file(fra);
      gtk_button_set_image(GTK_BUTTON(lista->botones[cascpu]),image);
      lista->turno--;
    }
  }

}

/**
*     Esta funcion permite regresar al tiro posterior, modificando
*     los datos y restableciendo al gif correspondiente.
*
*     @param data   Recibe los datos para llevar a cabo
*     la funcion.
*/
void tinext(GtkWidget *boton, gpointer data)
{
  nodo *lista;
  lista=(nodo *)data;
  int esp;
  int aux;
  aux=lista->turno;
  aux--;
  g_print("Antes del if\n");
  if((lista->modo!=0) && (lista->turno!=10) && (lista->tiros[aux] >=0) && (lista->vf==3))
  {
    g_print("Llegue a la funcion de next\n");
    esp=lista->tiros[aux];
    lista->ban=1;
    Boton(GTK_BUTTON(lista->botones[esp]), lista);
  }

}
