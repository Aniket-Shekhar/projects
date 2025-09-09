import tkinter as tk
import math

def click(button_text):
    current = entry.get()

    if button_text == "=":
        try:
            result = eval(current.replace("^", "**"))  # ^ replaced with power
            entry.delete(0, tk.END)
            entry.insert(tk.END, result)
        except:
            entry.delete(0, tk.END)
            entry.insert(tk.END, "Error")

    elif button_text == "C":
        entry.delete(0, tk.END)

    elif button_text == "√":
        try:
            value = float(current)
            entry.delete(0, tk.END)
            entry.insert(tk.END, math.sqrt(value))
        except:
            entry.delete(0, tk.END)
            entry.insert(tk.END, "Error")

    else:
        entry.insert(tk.END, button_text)

# Main window
root = tk.Tk()
root.title("Cool Python Calculator")
root.configure(bg="#242E38")  # Dark background

entry = tk.Entry(root, width=20, font=("Arial", 20, "bold"), bd=5, relief="solid", justify="right", bg="#ECF0F1", fg="#2C3E50")
entry.grid(row=0, column=0, columnspan=4, pady=10, padx=10)

# Button styles
button_colors = {
    "numbers": "#3A52B0",   # Blue
    "operators": "#A32A2A", # Orange
    "special": "#B8762C",   # Red
    "equals": "#239552"     # Green
}

# Buttons layout
buttons = [
    ("7", "numbers"), ("8", "numbers"), ("9", "numbers"), ("/", "operators"),
    ("4", "numbers"), ("5", "numbers"), ("6", "numbers"), ("*", "operators"),
    ("1", "numbers"), ("2", "numbers"), ("3", "numbers"), ("-", "operators"),
    ("0", "numbers"), (".", "numbers"), ("=", "equals"), ("+", "operators"),
    ("√", "special"), ("C", "special"), ("^", "operators")
]

row_val, col_val = 1, 0
for (button, btn_type) in buttons:
    color = button_colors.get(btn_type, "#BDC3C7")  # Default grey if not found
    action = lambda x=button: click(x)
    tk.Button(root, text=button, width=5, height=2,
              font=("Arial", 16, "bold"), bg=color, fg="white",
              command=action, relief="raised", bd=3).grid(row=row_val, column=col_val, padx=5, pady=5)

    col_val += 1
    if col_val > 3:
        col_val = 0
        row_val += 1

root.mainloop()
