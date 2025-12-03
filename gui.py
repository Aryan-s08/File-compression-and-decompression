import customtkinter as ctk
from tkinter import filedialog, messagebox
import subprocess
import os

# Executables
COMPRESS_EXEC = "/home/aub/Desktop/Projects/FCAD/compress2"
SECOND_EXEC = "/home/aub/Desktop/Projects/FCAD/de"

ctk.set_appearance_mode("system")  # "light", "dark", or "system"
ctk.set_default_color_theme("blue")


def select_input_file1():
    path = filedialog.askopenfilename(title="Select First Input File")
    if path:
        input_entry1.delete(0, "end")
        input_entry1.insert(0, path)


def select_input_file2():
    path = filedialog.askopenfilename(title="Select Second Input File")
    if path:
        input_entry2.delete(0, "end")
        input_entry2.insert(0, path)


def _run_exec(exec_path, args):
    # Basic checks on the executable
    if not os.path.exists(exec_path):
        return False, f"Executable not found:\n{exec_path}"

    if not os.access(exec_path, os.X_OK):
        return False, (
            f"Executable exists but is not marked as executable:\n{exec_path}\n\n"
            f"Try running:\nchmod +x \"{exec_path}\""
        )

    try:
        proc = subprocess.run(
            [exec_path] + args,
            capture_output=True,  # capture stdout and stderr
            text=True
        )
    except FileNotFoundError as e:
        return False, f"FileNotFoundError while starting program:\n{e}"
    except Exception as e:
        return False, f"Unexpected error while running program:\n{e}"

    stdout = (proc.stdout or "").strip()
    stderr = (proc.stderr or "").strip()

    if proc.returncode != 0:
        # Include both stdout and stderr to help debug
        msg = (
            f"Program exited with return code {proc.returncode}.\n\n"
            f"STDOUT:\n{stdout or '<none>'}\n\n"
            f"STDERR:\n{stderr or '<none>'}"
        )
        return False, msg
    else:
        msg = (
            f"Program finished successfully.\n\n"
            f"STDOUT:\n{stdout or '<none>'}\n\n"
            f"STDERR:\n{stderr or '<none>'}"
        )
        return True, msg


def run_processing1():
    input_path = input_entry1.get()
    string_len = string_len_entry.get()
    displacement = displacement_entry.get()

    if not os.path.exists(input_path):
        messagebox.showerror("Error", "Please select a valid input file.")
        return

    if not string_len.isdigit() or not displacement.isdigit():
        messagebox.showerror("Error", "Max string length and displacement must be numbers.")
        return

    ok, msg = _run_exec(COMPRESS_EXEC, [input_path, string_len, displacement])
    if not ok:
        messagebox.showerror("Program Error", msg)
    else:
        messagebox.showinfo("Success", "Compression completed!\n\n" + msg)


def run_processing2():
    input_path = input_entry2.get()

    if not os.path.exists(input_path):
        messagebox.showerror("Error", "Please select a valid second input file.")
        return

    ok, msg = _run_exec(SECOND_EXEC, [input_path])
    if not ok:
        messagebox.showerror("Program Error", msg)
    else:
        messagebox.showinfo("Success", "Decompression completed!\n\n" + msg)


# ----------------------------------------------------
# GUI MAIN WINDOW
# ----------------------------------------------------
app = ctk.CTk()
app.title("File Compression and Decompression")
app.geometry("520x570")   # increased height for extra spacing

title = ctk.CTkLabel(app, text="File Compression and Decompression", font=("Arial", 22, "bold"))
title.pack(pady=15)

# ------------------- First Program Section -------------------------
frame1 = ctk.CTkFrame(app)
frame1.pack(pady=10, padx=20, fill="x")

ctk.CTkLabel(frame1, text="Select First Input File:", font=("Arial", 14)).pack(pady=5)

file_frame1 = ctk.CTkFrame(frame1)
file_frame1.pack(pady=5)

input_entry1 = ctk.CTkEntry(file_frame1, width=280)
input_entry1.pack(side="left", padx=5)

browse_btn1 = ctk.CTkButton(file_frame1, text="Browse", width=90, command=select_input_file1)
browse_btn1.pack(side="left")

ctk.CTkLabel(frame1, text="Max String Length (x):", font=("Arial", 14)).pack(pady=5)
string_len_entry = ctk.CTkEntry(frame1, width=120)
string_len_entry.pack()

ctk.CTkLabel(frame1, text="Max Displacement (y):", font=("Arial", 14)).pack(pady=5)
displacement_entry = ctk.CTkEntry(frame1, width=120)
displacement_entry.pack()

run_btn1 = ctk.CTkButton(frame1, text="Compress File", width=200, command=run_processing1)
run_btn1.pack(pady=15)

# ------------------- Spacer -------------------------
ctk.CTkLabel(app, text="", height=5).pack(pady=20)

# ------------------- Second Program Section -------------------------
frame2 = ctk.CTkFrame(app)
frame2.pack(pady=10, padx=20, fill="x")

ctk.CTkLabel(frame2, text="Select Second Input File:", font=("Arial", 14)).pack(pady=5)

file_frame2 = ctk.CTkFrame(frame2)
file_frame2.pack(pady=5)

input_entry2 = ctk.CTkEntry(file_frame2, width=280)
input_entry2.pack(side="left", padx=5)

browse_btn2 = ctk.CTkButton(file_frame2, text="Browse", width=90, command=select_input_file2)
browse_btn2.pack(side="left")

run_btn2 = ctk.CTkButton(frame2, text="Decompress File", width=200, command=run_processing2)
run_btn2.pack(pady=15)

app.mainloop()
