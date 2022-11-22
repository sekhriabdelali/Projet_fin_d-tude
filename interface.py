from tkinter import *
import pymysql
from datetime import datetime
import time
from tkinter import *
import csv
import openpyxl
from sqlalchemy import create_engine
import pandas
import threading
import sys
from tkinter import messagebox
from mailer import Mailer


class database:
    def __init__(self, host, user, password, database):
        self.host = host
        self.user = user
        self.password = password
        self.database = database
        self.csv_data = []

    def connect_db(self):
        return pymysql.connect(
            host=self.host,
            user=self.user,
            password=self.password,
            database=self.database
        )

    def temp_hum_last(self, table):
        connection = self.connect_db()
        with connection:
            with connection.cursor() as cursor:
                request = f"SELECT * FROM `{table}` ORDER BY `id` DESC LIMIT 1"
                cursor.execute(request)
                result = cursor.fetchall()
        return result[0]


def csv_file1():
    # create csv file.
    file = open(fr"C:\Users\Dell2021\Desktop\ESP1.csv", "w").close()
    open(fr"C:\Users\Dell2021\Desktop\ESP1.xlsx", "w").close()
    my_conn = create_engine(
        f"mysql+pymysql://billel:password@192.168.5.3/esp_data")

    sql = f"select * from esp1"
    my_data = pandas.read_sql(sql, my_conn)
    my_data.to_csv(fr"C:\Users\Dell2021\Desktop\ESP1.csv", index=False)
    # fermiture
    my_conn.dispose()
    with open(fr"C:\Users\Dell2021\Desktop\ESP1.csv") as file_obj:
        csv_data = []
        reader = csv.reader(file_obj)
        for row in reader:
            csv_data.append(row)

    wb = openpyxl.Workbook()
    sheet = wb.active
    for row in csv_data:
        sheet.append(row)
    wb.save(fr"C:\Users\Dell2021\Desktop\ESP1.xlsx")


def csv_file2():
    # create csv file.
    file = open(fr"C:\Users\Dell2021\Desktop\ESP2.csv", "w").close()
    open(fr"C:\Users\Dell2021\Desktop\ESP2.xlsx", "w").close()
    my_conn = create_engine(
        f"mysql+pymysql://billel:password@192.168.5.3/esp_data")

    sql = f"select * from esp2"
    my_data = pandas.read_sql(sql, my_conn)
    my_data.to_csv(fr"C:\Users\Dell2021\Desktop\ESP2.csv", index=False)
    # fermiture
    my_conn.dispose()
    with open(fr"C:\Users\Dell2021\Desktop\ESP2.csv") as file_obj:
        csv_data = []
        reader = csv.reader(file_obj)
        for row in reader:
            csv_data.append(row)

    wb = openpyxl.Workbook()
    sheet = wb.active
    for row in csv_data:
        sheet.append(row)
    wb.save(fr"C:\Users\Dell2021\Desktop\ESP2.xlsx")
# -------------- send_mail(): -------------------


def send_mail(num):
    print(f"Dépêchez-vous {num}")
    mail = Mailer(email="gtrpfe@gmail.com", password="qazhqjsgdulgdyzu")
    mail.send(receiver="alisekhri46@gmail.com",
              subject="Alerte", message=f"Dépêchez-vous, le feu s'est déclaré dans le noeud{num}!!!")
    print(f"Dépêchez-vous, le feu s'est déclaré dans le noeud{num}!!!")

# -------------- warnning1(): -------------------


def esp1_val():
    try:
        while True:
            val = dat.temp_hum_last("esp1")
            date_var1.set(str(val[1]))
            temp_var1.set(val[2] + " °C")
            hum_var1.set(val[3] + " %")
            qua_var1.set(val[4] + " ppm")
            if(float(val[4]) > 300.00 and float(val[2]) > 30.00):
                messagebox.showwarning(
                    "warnning", "le feu s'est déclaré dans le noeud 1 ")
                # threading.Thread(send_mail(1)).start()
            time.sleep(5)
    except:
        #sys.exit("we exit")
        pass

# -------------- warnning2(): -------------------


def esp2_val():
    try:

        while True:
            val = dat.temp_hum_last("esp2")
            date_var2.set(str(val[1]))
            temp_var2.set(val[2] + " °C")
            hum_var2.set(val[3] + " %")
            qua_var2.set(val[4] + " ppm")
            if(float(val[4]) > 300.00 and float(val[2]) > 30.00):
                messagebox.showwarning(
                    "warnning", "le feu s'est déclaré dans le noeud 2 ")
                # threading.Thread(send_mail(2)).start()
            time.sleep(5)
    except:
        #sys.exit("we exit")
        pass


dat = database("192.168.5.3", "billel", "password", "esp_data")

pro = Tk()


# --------------- variables des entrees de esp 1: --------------
date_var1 = StringVar()
temp_var1 = StringVar()
hum_var1 = StringVar()
qua_var1 = StringVar()

# --------------- variables des entrees de esp 2: --------------
date_var2 = StringVar()
temp_var2 = StringVar()
hum_var2 = StringVar()
qua_var2 = StringVar()


# -------------- esp1(): -------------------


def esp1():

    pro.title("ESP 1")

    # oublier le main:

    label1.place_forget()

    btn1.place_forget()

    btn2.place_forget()

    # placer le titre:

    title1.place(x=0, y=0, height=50, width=800)

    # placer les labels:

    lbl_date1.place(x=30, y=90, width=150, height=40)

    lbl_temp1.place(x=217, y=90, width=140, height=40)

    lbl_hum1.place(x=404, y=90, width=140, height=40)

    lbl_qua1.place(x=591, y=90, width=180, height=40)

    # placer les boutons:

    btn_back1.place(x=30, y=300, width=200, height=40)

    btn_excel1.place(x=420, y=300, width=350, height=40)

    # placer les entrees:

    ent_date1.place(x=30, y=170, width=150, height=40)

    ent_temp1.place(x=217, y=170, width=140, height=40)

    ent_hum1.place(x=404, y=170, width=140, height=40)

    ent_qua1.place(x=591, y=170, width=180, height=40)


# -------------- esp2(): -------------------


def esp2():

    pro.title("ESP 2")
    # pro.iconbitmap(r"C:\Users\Dell2021\Documents\PythonScripts\pfe\esp.ico")

    # oublier le main:

    label1.place_forget()

    btn1.place_forget()

    btn2.place_forget()

    # placer le titre:   #1e3d59

    title2.place(x=0, y=0, height=50, width=800)

    # placer les labels:

    lbl_date2.place(x=30, y=90, width=150, height=40)

    lbl_temp2.place(x=217, y=90, width=140, height=40)

    lbl_hum2.place(x=404, y=90, width=140, height=40)

    lbl_qua2.place(x=591, y=90, width=180, height=40)

    # placer les boutons:

    btn_back2.place(x=30, y=300, width=200, height=40)

    btn_excel2.place(x=420, y=300, width=350, height=40)

    # placer les entrees:

    ent_date2.place(x=30, y=170, width=150, height=40)

    ent_temp2.place(x=217, y=170, width=140, height=40)

    ent_hum2.place(x=404, y=170, width=140, height=40)

    ent_qua2.place(x=591, y=170, width=180, height=40)


def back_main():
    # placer le titre:
    pro.title("Station")

    # placer la background et les labels et les buttons du main:

    label1.place(x=0, y=0)
    btn1.place(x=625, y=166.2, width=65, height=30)
    btn2.place(x=625, y=345, width=65, height=30)

    # oublier esp1:

    title1.place_forget()
    lbl_date1.place_forget()
    lbl_temp1.place_forget()
    lbl_hum1.place_forget()
    lbl_qua1.place_forget()
    btn_back1.place_forget()
    btn_excel1.place_forget()
    ent_date1.place_forget()
    ent_temp1.place_forget()
    ent_hum1.place_forget()
    ent_qua1.place_forget()

    # oublier esp2:

    title2.place_forget()
    lbl_date2.place_forget()
    lbl_temp2.place_forget()
    lbl_hum2.place_forget()
    lbl_qua2.place_forget()
    btn_back2.place_forget()
    btn_excel2.place_forget()
    ent_date2.place_forget()
    ent_temp2.place_forget()
    ent_hum2.place_forget()
    ent_qua2.place_forget()


# ---------------window: --------------
threading.Thread(target=esp1_val).start()
threading.Thread(target=esp2_val).start()

pro.title("Station")
pro.geometry("800x400")
pro.resizable(False, False)
pro.iconbitmap(
    r"C:\Users\Dell2021\Documents\PythonScripts\pfe\image\weather.ico")

# -------------- bg: -------------------
bg = PhotoImage(
    file=r"C:\Users\Dell2021\Documents\PythonScripts\pfe\image\background_pfe.png")
#res = bg.subsample(7, 7)
label1 = Label(pro, image=bg)
label1.place(x=0, y=0)

# -------------- buttons: -------------------

btn1 = Button(pro, text="Noeud1", font=(
    "Myriad Arabic", 12), bg="white", bd=0, command=esp1)
btn1.place(x=625, y=166.2, width=65, height=30)

btn2 = Button(pro, text="Noeud2", font=(
    "Myriad Arabic", 12), bg="white", bd=0, command=esp2)
btn2.place(x=625, y=345, width=65, height=30)


# -------------- title esp 1: -------------------

title1 = Label(pro, text="Le noeud (ESP 1)",
               font=("Myriad Arabic", 16), bg="#1e3d59", fg="white")

# -------------- labels esp 1: -------------------

lbl_date1 = Label(pro, text="Date", font=("Myriad Arabic", 15), bg="#f5f0e1")

lbl_temp1 = Label(pro, text="Temperature", font=(
    "Myriad Arabic", 15), bg="#f5f0e1")

lbl_hum1 = Label(pro, text="Humidité", font=(
    "Myriad Arabic", 15), bg="#f5f0e1")

lbl_qua1 = Label(pro, text="Qualité d'aire",
                 font=("Myriad Arabic", 15), bg="#f5f0e1")

# -------------- buttons esp 1: -------------------

btn_back1 = Button(pro, text="revenir au main",
                   font=("Myriad Arabic", 15), bg="#f5f0e1", command=back_main)

btn_excel1 = Button(pro, text="générer un fichier excel",
                    font=("Myriad Arabic", 15), bg="#f5f0e1", command=csv_file1)

# -------------- entry esp 1: -------------------
ent_date1 = Entry(pro, bd="2", justify="center", textvariable=date_var1)

ent_temp1 = Entry(pro, bd="2", justify="center", textvariable=temp_var1)

ent_hum1 = Entry(pro, bd="2", justify="center", textvariable=hum_var1)

ent_qua1 = Entry(pro, bd="2", justify="center", textvariable=qua_var1)


# -------------- title esp 2: -------------------

title2 = Label(pro, text="Le noeud (ESP 2)",
               font=("Myriad Arabic", 16), bg="#1e3d59", fg="white")

pro.config(background="#F8F9F9")
# -------------- labels esp 2: -------------------   #f5f0e1

lbl_date2 = Label(pro, text="Date", font=("Myriad Arabic", 15), bg="#f5f0e1")

lbl_temp2 = Label(pro, text="Temperature", font=(
    "Myriad Arabic", 15), bg="#f5f0e1")

lbl_hum2 = Label(pro, text="Humidité", font=(
    "Myriad Arabic", 15), bg="#f5f0e1")

lbl_qua2 = Label(pro, text="Qualité d'aire",
                 font=("Myriad Arabic", 15), bg="#f5f0e1")

# -------------- buttons esp 2: -------------------

btn_back2 = Button(pro, text="revenir au main",
                   font=("Myriad Arabic", 15), bg="#f5f0e1", command=back_main)

btn_excel2 = Button(pro, text="générer un fichier excel",
                    font=("Myriad Arabic", 15), bg="#f5f0e1", command=csv_file2)

# -------------- entry esp 2: -------------------
ent_date2 = Entry(pro, bd="2", justify="center", textvariable=date_var2)

ent_temp2 = Entry(pro, bd="2", justify="center", textvariable=temp_var2)

ent_hum2 = Entry(pro, bd="2", justify="center", textvariable=hum_var2)

ent_qua2 = Entry(pro, bd="2", justify="center", textvariable=qua_var2)
# -----------------------------------end of pro ------------------------------
pro.mainloop()


# pour rendre le code exécutable, on tape cette commande : pyinstaller.exe --onefile --windowed -i icone_name.ico python_file_name.py
