import tkinter as tk
from tkinter import messagebox
import random

current_player = "X"  # Human = X, Computer = O

def button_click(btn):
    global current_player
    if btn["text"] == "" and current_player == "X":
        btn["text"] = "X"
        if check_winner("X"):
            messagebox.showinfo("Game Over", "You Win! 🎉")
            reset_game()
            return
        elif board_full():
            messagebox.showinfo("Game Over", "It's a Draw! 🤝")
            reset_game()
            return
        
        current_player = "O"
        computer_move()

def computer_move():
    global current_player
    empty_buttons = [b for row in buttons for b in row if b["text"] == ""]

    # 1. If computer can win, take it
    for b in empty_buttons:
        b["text"] = "O"
        if check_winner("O"):
            return end_game("O")
        b["text"] = ""

    # 2. If player is about to win, block
    for b in empty_buttons:
        b["text"] = "X"
        if check_winner("X"):
            b["text"] = "O"
            current_player = "X"
            return
        b["text"] = ""

    # 3. Otherwise random move
    if empty_buttons:
        choice = random.choice(empty_buttons)
        choice["text"] = "O"
        if check_winner("O"):
            return end_game("O")
        elif board_full():
            messagebox.showinfo("Game Over", "It's a Draw! 🤝")
            reset_game()
            return

    current_player = "X"

def check_winner(player):
    # Rows
    for row in buttons:
        if row[0]["text"] == row[1]["text"] == row[2]["text"] == player:
            return True
    # Columns
    for col in range(3):
        if buttons[0][col]["text"] == buttons[1][col]["text"] == buttons[2][col]["text"] == player:
            return True
    # Diagonals
    if buttons[0][0]["text"] == buttons[1][1]["text"] == buttons[2][2]["text"] == player:
        return True
    if buttons[0][2]["text"] == buttons[1][1]["text"] == buttons[2][0]["text"] == player:
        return True
    return False

def board_full():
    return all(b["text"] != "" for row in buttons for b in row)

def end_game(winner):
    if winner == "O":
        messagebox.showinfo("Game Over", "Computer Wins! 🤖")
    elif winner == "X":
        messagebox.showinfo("Game Over", "You Win! 🎉")
    reset_game()

def reset_game():
    global current_player
    current_player = "X"
    for row in buttons:
        for b in row:
            b.config(text="")

# GUI Setup
root = tk.Tk()
root.title("Tic-Tac-Toe vs Computer 🎮")
root.geometry("300x350")
root.config(bg="#f4f4f9")

label = tk.Label(root, text="Tic-Tac-Toe (You vs Computer)", font=("Arial", 14, "bold"), bg="#f4f4f9")
label.pack(pady=10)

frame = tk.Frame(root)
frame.pack()

buttons = [[None]*3 for _ in range(3)]

for i in range(3):
    for j in range(3):
        btn = tk.Button(frame, text="", font=("Arial", 18), width=5, height=2,
                        command=lambda b=(i, j): button_click(buttons[b[0]][b[1]]))
        btn.grid(row=i, column=j)
        buttons[i][j] = btn

reset_btn = tk.Button(root, text="Reset Game", command=reset_game, bg="#2196f3", fg="white")
reset_btn.pack(pady=10)

root.mainloop()
