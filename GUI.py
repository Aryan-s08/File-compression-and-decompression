import customtkinter as ctk
from tkinter import filedialog, messagebox
import subprocess
import os

# Executables
COMPRESS_EXEC = "/home/aub/Desktop/Projects/FCAD/compress"
SECOND_EXEC = "/home/aub/Desktop/Projects/FCAD/de"

ctk.set_appearance_mode("system")
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


# ----------------------------------------------
# Helper to run .exe programs
# ----------------------------------------------
def _run_exec(exec_path, args):
    if not os.path.exists(exec_path):
        return False, f"Executable not found:\n{exec_path}"

    try:
        proc = subprocess.run(
            [exec_path] + args,
            capture_output=True,
            text=True
        )
    except Exception as e:
        return False, f"Error while running program:\n{e}"

    stdout = (proc.stdout or "").strip()
    stderr = (proc.stderr or "").strip()

    if proc.returncode != 0:
        msg = (f"Program exited with return code {proc.returncode}.\n\n"
               f"STDOUT:\n{stdout or '<none>'}\n\n"
               f"STDERR:\n{stderr or '<none>'}")
        return False, msg
    else:
        msg = (f"Program finished successfully.\n\n"
               f"STDOUT:\n{stdout or '<none>'}\n\n"
               f"STDERR:\n{stderr or '<none>'}")
        return True, msg


# ----------------------------------------------------
# Manual slider run
# ----------------------------------------------------
def run_slider_values():
    input_path = input_entry1.get()

    if not os.path.exists(input_path):
        messagebox.showerror("Error", "Please select a valid input file.")
        return

    x = int(size_slider.get())
    y = int(distance_slider.get())

    ok, msg = _run_exec(COMPRESS_EXEC, [input_path, str(x), str(y)])
    if not ok:
        messagebox.showerror("Program Error", msg)
    else:
        messagebox.showinfo("Success", f"Compression done.\n\n{x=}, {y=}\n\n" + msg)


# ----------------------------------------------------
# Preset button runners
# ----------------------------------------------------
def run_fixed(x, y):
    input_path = input_entry1.get()

    if not os.path.exists(input_path):
        messagebox.showerror("Error", "Please select a valid input file.")
        return

    ok, msg = _run_exec(COMPRESS_EXEC, [input_path, str(x), str(y)])
    if not ok:
        messagebox.showerror("Program Error", msg)
    else:
        messagebox.showinfo("Success", f"Compression done with preset.\n\nx={x}, y={y}\n\n" + msg)


# ----------------------------------------------------
# Second (decompression) program
# ----------------------------------------------------
def run_processing2():
    input_path = input_entry2.get()

    if not os.path.exists(input_path):
        messagebox.showerror("Error", "Please select a valid compressed file.")
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
app.geometry("550x750")

title = ctk.CTkLabel(app, text="File Compression and Decompression",
                     font=("Arial", 22, "bold"))
title.pack(pady=15)


# ------------------- First Program Section -------------------------
frame1 = ctk.CTkFrame(app)
frame1.pack(pady=10, padx=20, fill="x")

ctk.CTkLabel(frame1, text="Select Input File:", font=("Arial", 14)).pack(pady=5)

file_frame1 = ctk.CTkFrame(frame1)
file_frame1.pack(pady=5)

input_entry1 = ctk.CTkEntry(file_frame1, width=300)
input_entry1.pack(side="left", padx=5)

browse_btn1 = ctk.CTkButton(file_frame1, text="Browse", width=90, command=select_input_file1)
browse_btn1.pack(side="left")


# ------------------- Sliders -------------------------

def update_size_label(value):
    size_label.configure(text=f"Selected: {int(float(value))}")

def update_disp_label(value):
    disp_label.configure(text=f"Selected: {int(float(value))}")


ctk.CTkLabel(frame1, text="Max String Length (x):", font=("Arial", 14)).pack(pady=5)
size_slider = ctk.CTkSlider(frame1, from_=1, to=6, number_of_steps=5,
                            width=260, command=update_size_label)
size_slider.set(4)
size_slider.pack(pady=5)

size_label = ctk.CTkLabel(frame1, text=f"Selected: {int(size_slider.get())}", font=("Arial", 12))
size_label.pack()

ctk.CTkLabel(frame1, text="Max Displacement (y):", font=("Arial", 14)).pack(pady=5)
distance_slider = ctk.CTkSlider(frame1, from_=1, to=20, number_of_steps=19,
                                width=260, command=update_disp_label)
distance_slider.set(8)
distance_slider.pack(pady=5)

disp_label = ctk.CTkLabel(frame1, text=f"Selected: {int(distance_slider.get())}", font=("Arial", 12))
disp_label.pack()


manual_btn = ctk.CTkButton(frame1, text="Compress (Slider Values)",
                           width=220, command=run_slider_values)
manual_btn.pack(pady=12)


# ------------------- Preset Buttons -------------------------
preset_frame = ctk.CTkFrame(frame1)
preset_frame.pack(pady=15)

ctk.CTkLabel(preset_frame, text="Preset Compression Settings:",
             font=("Arial", 14, "bold")).pack(pady=5)

btn1 = ctk.CTkButton(preset_frame, text="Fast Compression",
                     width=200, command=lambda: run_fixed(2, 5))
btn1.pack(pady=5)

btn2 = ctk.CTkButton(preset_frame, text="Normal Compression",
                     width=200, command=lambda: run_fixed(4, 14))
btn2.pack(pady=5)

btn3 = ctk.CTkButton(preset_frame, text="Slow Compression",
                     width=200, command=lambda: run_fixed(5, 18))
btn3.pack(pady=5)


# ------------------- Spacer -------------------------
ctk.CTkLabel(app, text="", height=10).pack(pady=10)


# ------------------- Second Program Section -------------------------
frame2 = ctk.CTkFrame(app)
frame2.pack(pady=10, padx=20, fill="x")

ctk.CTkLabel(frame2, text="Select File to Decompress:", font=("Arial", 14)).pack(pady=5)

file_frame2 = ctk.CTkFrame(frame2)
file_frame2.pack(pady=5)

input_entry2 = ctk.CTkEntry(file_frame2, width=300)
input_entry2.pack(side="left", padx=5)

browse_btn2 = ctk.CTkButton(file_frame2, text="Browse", width=90, command=select_input_file2)
browse_btn2.pack(side="left")

run_btn2 = ctk.CTkButton(frame2, text="Decompress File",
                         width=200, command=run_processing2)
run_btn2.pack(pady=15)


app.mainloop()

