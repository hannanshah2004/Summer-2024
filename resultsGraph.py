import matplotlib.pyplot as plt
import numpy as np
import os


# Function to read data from a text file and calculate average validation accuracy per epoch
def read_data(file_path):
    epoch_acc = {}
    
    with open(file_path, 'r') as file:
        lines = file.readlines()

    for line in lines:
        if 'val acc:' in line:
            parts = line.strip().split()
            epoch = int(parts[1])
            val_acc = float(parts[-1])
            
            if epoch not in epoch_acc:
                epoch_acc[epoch] = []
            epoch_acc[epoch].append(val_acc)

    # Calculate average validation accuracy per epoch
    epochs = sorted(epoch_acc.keys())
    avg_val_accs = [sum(epoch_acc[e]) / len(epoch_acc[e]) for e in epochs]

    return epochs, avg_val_accs

# Function to plot the validation accuracy against epochs for each aggregation strategy
def plot_accuracy(data, attack_type, malicious_ratio, save_path):
    plt.figure(figsize=(14, 8))
    plt.style.use('Solarize_Light2')  # Set the style

    line_styles = ['-', '--', '-.', ':', '-', '--', '-.', ':']  # Define line styles
    colors = plt.cm.viridis(np.linspace(0, 1, len(data)))  # Use a colormap for distinct colors
    line_style_index = 0

    for strategy, values in data.items():
        epochs, val_accs = values
        plt.plot(epochs, val_accs, label=strategy, color=colors[line_style_index], linestyle=line_styles[line_style_index % len(line_styles)], linewidth=2)
        line_style_index += 1

    plt.xlabel('Epochs', fontsize=14, fontweight='bold')
    plt.ylabel('Validation Accuracy', fontsize=14, fontweight='bold')
    plt.title(f'Validation Accuracy vs Epochs for {attack_type} \nMalicious Ratio: {malicious_ratio}%', fontsize=16, fontweight='bold')
        
    # Customize legend
    plt.legend(loc='upper left', fontsize=12, frameon=False)
    
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(save_path)
    plt.close()

# Main function to process files and plot the data
def main():
    # Set the attack type and the malicious ratio
    attack_type = "scalingAttack"
    malicious_ratio = 45  # Hardcoded malicious ratio
    base_dir = "Results/45Percent/ScalingAttack"  # Change this to the directory where your text files are located

    # Dictionary to store data for each aggregation strategy
    data = {}
    
    # Process each text file in the directory
    for filename in os.listdir(base_dir):
        if filename.startswith(attack_type) and filename.endswith(".txt"):
            strategy_name = filename.replace(f'{attack_type}_', '').replace('.txt', '')
            file_path = os.path.join(base_dir, filename)
            epochs, avg_val_accs = read_data(file_path)
            data[strategy_name] = (epochs, avg_val_accs)
    
    # Plot the data and save the plot as an image
    save_path = os.path.join(base_dir, f'{attack_type}_{malicious_ratio}.png')
    plot_accuracy(data, attack_type, malicious_ratio, save_path)
    print(f"Plot saved as {save_path}")

if __name__ == "__main__":
    main()
