using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace ClienteSockets
{
    public partial class Form1 : Form
    {
        Socket server;
        int conectado = 0;//Para saber si estamos conectados al servidor o no
        int login = 0;//Para saber si el usuario ha iniciado sesión
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                // Definimos la dirección y el puerto del servidor
                IPAddress ip = IPAddress.Parse("192.168.56.102");
                IPEndPoint ipep = new IPEndPoint(ip, 9050);

                // Creamos el socket TCP
                server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                // Intentamos conectar
                server.Connect(ipep);

                // Cambiamos el fondo a verde para indicar conexión exitosa
                this.BackColor = System.Drawing.Color.Green;
                this.conectado = 1;
                MessageBox.Show("Conectado al servidor");


            }
            catch (Exception ex)
            {
                MessageBox.Show("Error al conectar con el servidor: " + ex.Message);
            }
        }

        private void buttonLogIn_Click(object sender, EventArgs e)
        {
            if (this.conectado == 1)
            {
                if (Username1TextBox.Text == null || Password1TextBox.Text == null)
                {
                    MessageBox.Show("No puedes realizar el inicio de sesion sin haber puesto un nombre de usuario o contraseña!");
                }
                else
                {
                    string mensaje = "9/" + Username1TextBox.Text + "/" + Password1TextBox.Text;
                    //Enviamos al servidor el nombre y contraseña tecleados
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    if (mensaje == "Login exitoso.")
                    {
                        MessageBox.Show("Has inciado sesión con éxito.");
                        this.login = 1;
                    }
                    else if (mensaje == "Usuario o contraseña incorrectos.")
                    {
                        MessageBox.Show("El nombre de usuario o contraseña son incorrectos.");
                    }
                    else if (mensaje == "Error en la consulta.")
                    {
                        MessageBox.Show("Ha ocurrido un error en la consulta.");
                    }
                }


            }
            else
            {
                MessageBox.Show("¡Antes de poder realizar cualquier acción debes conectarte al servidor!");
            }
        }

        private void Password1TextBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void buttonRegister_Click(object sender, EventArgs e)
        {
            if (this.conectado == 1)
            {
                if (Username2TextBox.Text == null || Password2TextBox.Text == null)
                {
                    MessageBox.Show("No puedes realizar el inicio de sesion sin haber puesto un nombre de usuario o contraseña!");
                    return;
                }
                else
                {
                    string mensaje = "0/" + Username2TextBox.Text + "/" + Password2TextBox.Text;
                    //Enviamos al servidor el nombre y contraseña tecleados
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    if (mensaje == "Registro exitoso.")
                    {
                        MessageBox.Show("Te has registrado con éxito.");
                    }
                    else if (mensaje == "Error en el registro.")
                    {
                        MessageBox.Show("Error en el registro");
                    }
                    return;
                }
            }
            else
            {
                MessageBox.Show("¡Antes de poder realizar cualquier acción debes conectarte al servidor!");
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (this.conectado == 1)
            {
                if(this.login==1)
                {
                    string mensaje = "3/" + Username1TextBox.Text + "/" + Password1TextBox.Text;
                    //Enviamos al servidor el nombre y contraseña tecleados
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    if (mensaje == "Error en la consulta.")
                    {
                        MessageBox.Show("Ha habido un error en la consulta.");
                    }
                    else
                    {
                        MessageBox.Show(mensaje);
                    }
                }
                else
                {
                    MessageBox.Show("¡Antes de poder realizar una consulta debes iniciar sesión!");
                }
                             
            }
            else
            {
                MessageBox.Show("¡Antes de poder realizar cualquier acción debes conectarte al servidor!");
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (this.conectado == 1)
            {
                if (this.login == 1)
                {
                    string mensaje = "1/" + Username1TextBox.Text + "/" + Password1TextBox.Text;
                    //Enviamos al servidor el nombre y contraseña tecleados
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    if (mensaje == "Error en la consulta.")
                    {
                        MessageBox.Show("Ha habido un error en la consulta.");
                    }
                    else
                    {
                        MessageBox.Show(mensaje);
                    }
                }
                else
                {
                    MessageBox.Show("¡Antes de poder realizar una consulta debes iniciar sesión!");
                }

            }
            else
            {
                MessageBox.Show("¡Antes de poder realizar cualquier acción debes conectarte al servidor!");
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (this.conectado == 1)
            {
                if (this.login == 1)
                {
                    string mensaje = "2/" + Username1TextBox.Text + "/" + Password1TextBox.Text;
                    //Enviamos al servidor el nombre y contraseña tecleados
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    if (mensaje == "Error en la consulta.")
                    {
                        MessageBox.Show("Ha habido un error en la consulta.");
                    }
                    else
                    {
                        MessageBox.Show(mensaje);
                    }
                }
                else
                {
                    MessageBox.Show("¡Antes de poder realizar una consulta debes iniciar sesión!");
                }

            }
            else
            {
                MessageBox.Show("¡Antes de poder realizar cualquier acción debes conectarte al servidor!");
            }
        }
    }
}



