import tkinter as tk
import random
import string
from tkinter import messagebox

def generate_password():
    try:
        length = int(length_entry.get())
        if length < 6:
            messagebox.showwarning("Too Short", "Password length should be at least 6!")
            return
        
        # Characters pool
        letters = string.ascii_letters
        digits = string.digits
        symbols = string.punctuation
        all_chars = letters + digits + symbols

        # Ensure password contains at least one of each
        password = random.choice(letters) + random.choice(digits) + random.choice(symbols)
        password += "".join(random.choice(all_chars) for _ in range(length - 3))

        # Shuffle to avoid pattern
        password_list = list(password)
        random.shuffle(password_list)
        password = "".join(password_list)

        result_entry.delete(0, tk.END)
        result_entry.insert(0, password)

    except ValueError:
        messagebox.showerror("Invalid Input", "Please enter a number for length.")

# GUI setup
root = tk.Tk()
root.title("Cool Password Generator")
root.geometry("400x250")
root.config(bg="#2C3E50")

title = tk.Label(root, text="🔑 Password Generator", font=("Arial", 18, "bold"), fg="white", bg="#2C3E50")
title.pack(pady=10)

length_label = tk.Label(root, text="Enter Password Length:", font=("Arial", 12), fg="white", bg="#2C3E50")
length_label.pack()

length_entry = tk.Entry(root, font=("Arial", 12), justify="center")
length_entry.pack(pady=5)

generate_btn = tk.Button(root, text="Generate", font=("Arial", 14, "bold"),
                         bg="#3498DB", fg="white", relief="raised", bd=3,
                         command=generate_password)
generate_btn.pack(pady=10)

result_entry = tk.Entry(root, font=("Arial", 14), justify="center", width=30, bd=3, relief="solid")
result_entry.pack(pady=10)

root.mainloop()
