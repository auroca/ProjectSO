namespace ClienteSockets
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.Username1TextBox = new System.Windows.Forms.TextBox();
            this.Password1TextBox = new System.Windows.Forms.TextBox();
            this.buttonLogIn = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.Username2TextBox = new System.Windows.Forms.TextBox();
            this.Password2TextBox = new System.Windows.Forms.TextBox();
            this.buttonRegister = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(427, 58);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(133, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "STATE: DISCONNECTED";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(192, 171);
            this.button1.Margin = new System.Windows.Forms.Padding(2);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(167, 19);
            this.button1.TabIndex = 1;
            this.button1.Text = "CONNECT TO THE SERVER";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Username1TextBox
            // 
            this.Username1TextBox.Location = new System.Drawing.Point(19, 23);
            this.Username1TextBox.Margin = new System.Windows.Forms.Padding(2);
            this.Username1TextBox.Name = "Username1TextBox";
            this.Username1TextBox.Size = new System.Drawing.Size(76, 20);
            this.Username1TextBox.TabIndex = 3;
            this.Username1TextBox.Text = "Username";
            // 
            // Password1TextBox
            // 
            this.Password1TextBox.Location = new System.Drawing.Point(110, 23);
            this.Password1TextBox.Margin = new System.Windows.Forms.Padding(2);
            this.Password1TextBox.Name = "Password1TextBox";
            this.Password1TextBox.Size = new System.Drawing.Size(76, 20);
            this.Password1TextBox.TabIndex = 4;
            this.Password1TextBox.Text = "Password";
            this.Password1TextBox.TextChanged += new System.EventHandler(this.Password1TextBox_TextChanged);
            // 
            // buttonLogIn
            // 
            this.buttonLogIn.Location = new System.Drawing.Point(71, 45);
            this.buttonLogIn.Margin = new System.Windows.Forms.Padding(2);
            this.buttonLogIn.Name = "buttonLogIn";
            this.buttonLogIn.Size = new System.Drawing.Size(56, 19);
            this.buttonLogIn.TabIndex = 5;
            this.buttonLogIn.Text = "LOG IN";
            this.buttonLogIn.UseVisualStyleBackColor = true;
            this.buttonLogIn.Click += new System.EventHandler(this.buttonLogIn_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(379, 256);
            this.button3.Margin = new System.Windows.Forms.Padding(2);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(171, 19);
            this.button3.TabIndex = 6;
            this.button3.Text = "NUMBER OF GAMES";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(379, 280);
            this.button4.Margin = new System.Windows.Forms.Padding(2);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(171, 19);
            this.button4.TabIndex = 7;
            this.button4.Text = "MAX POINTS";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(379, 311);
            this.button5.Margin = new System.Windows.Forms.Padding(2);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(171, 19);
            this.button5.TabIndex = 8;
            this.button5.Text = "GAME LOG";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // Username2TextBox
            // 
            this.Username2TextBox.Location = new System.Drawing.Point(11, 87);
            this.Username2TextBox.Margin = new System.Windows.Forms.Padding(2);
            this.Username2TextBox.Name = "Username2TextBox";
            this.Username2TextBox.Size = new System.Drawing.Size(76, 20);
            this.Username2TextBox.TabIndex = 9;
            this.Username2TextBox.Text = "Username";
            // 
            // Password2TextBox
            // 
            this.Password2TextBox.Location = new System.Drawing.Point(110, 87);
            this.Password2TextBox.Margin = new System.Windows.Forms.Padding(2);
            this.Password2TextBox.Name = "Password2TextBox";
            this.Password2TextBox.Size = new System.Drawing.Size(76, 20);
            this.Password2TextBox.TabIndex = 10;
            this.Password2TextBox.Text = "Password";
            // 
            // buttonRegister
            // 
            this.buttonRegister.Location = new System.Drawing.Point(63, 110);
            this.buttonRegister.Margin = new System.Windows.Forms.Padding(2);
            this.buttonRegister.Name = "buttonRegister";
            this.buttonRegister.Size = new System.Drawing.Size(72, 19);
            this.buttonRegister.TabIndex = 11;
            this.buttonRegister.Text = "REGISTER";
            this.buttonRegister.UseVisualStyleBackColor = true;
            this.buttonRegister.Click += new System.EventHandler(this.buttonRegister_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(600, 366);
            this.Controls.Add(this.buttonRegister);
            this.Controls.Add(this.Password2TextBox);
            this.Controls.Add(this.Username2TextBox);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.buttonLogIn);
            this.Controls.Add(this.Password1TextBox);
            this.Controls.Add(this.Username1TextBox);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox Username1TextBox;
        private System.Windows.Forms.TextBox Password1TextBox;
        private System.Windows.Forms.Button buttonLogIn;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.TextBox Username2TextBox;
        private System.Windows.Forms.TextBox Password2TextBox;
        private System.Windows.Forms.Button buttonRegister;
    }
}

