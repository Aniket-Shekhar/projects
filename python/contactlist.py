import tkinter as tk
from tkinter import messagebox
import json

CONTACT_FILE = "contacts.json"

# Load and Save Functions
def load_contacts():
    try:
        with open(CONTACT_FILE, "r") as file:
            return json.load(file)
    except FileNotFoundError:
        return {}

def save_contacts(contacts):
    with open(CONTACT_FILE, "w") as file:
        json.dump(contacts, file, indent=4)

# Add Contact
def add_contact():
    name = name_entry.get().strip()
    phone = phone_entry.get().strip()

    if name and phone:
        contacts = load_contacts()
        contacts[name] = phone
        save_contacts(contacts)
        messagebox.showinfo("Success", f"Contact {name} added!")
        name_entry.delete(0, tk.END)
        phone_entry.delete(0, tk.END)
        view_contacts()
    else:
        messagebox.showwarning("Input Error", "Please enter both name and phone.")

# View Contacts
def view_contacts():
    contact_list.delete(0, tk.END)
    contacts = load_contacts()
    for name, phone in contacts.items():
        contact_list.insert(tk.END, f"{name}: {phone}")

# Delete Contact
def delete_contact():
    selected = contact_list.curselection()
    if selected:
        contact_text = contact_list.get(selected[0])
        name = contact_text.split(":")[0].strip()
        contacts = load_contacts()
        if name in contacts:
            del contacts[name]
            save_contacts(contacts)
            messagebox.showinfo("Deleted", f"Contact {name} deleted.")
            view_contacts()
    else:
        messagebox.showwarning("Selection Error", "Please select a contact to delete.")

# GUI Setup
root = tk.Tk()
root.title("Contact List App")
root.geometry("400x450")
root.config(bg="#0a0a3d")

# Input Fields
tk.Label(root, text="Name:", bg="#ff3535").pack(pady=5)
name_entry = tk.Entry(root, width=40)
name_entry.pack(pady=5)

tk.Label(root, text="Phone:", bg="#4728d0").pack(pady=5)
phone_entry = tk.Entry(root, width=40)
phone_entry.pack(pady=5)

tk.Button(root, text="Add Contact", command=add_contact, bg="#4caf50", fg="white").pack(pady=10)

# Contact List
contact_list = tk.Listbox(root, width=50, height=10)
contact_list.pack(pady=10)

tk.Button(root, text="Delete Contact", command=delete_contact, bg="#f44336", fg="white").pack(pady=5)

tk.Button(root, text="Refresh List", command=view_contacts, bg="#2196f3", fg="white").pack(pady=5)

# Initialize
view_contacts()
root.mainloop()
