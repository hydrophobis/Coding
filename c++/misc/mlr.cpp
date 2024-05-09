#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include <cassert>
#include <ctime>
#include <functional>

// Sigmoid activation function
double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

// Derivative of sigmoid function
double sigmoid_derivative(double x) {
    return sigmoid(x) * (1 - sigmoid(x));
}

// Structure for a single training data point
struct DataPoint {
    std::vector<double> inputs; // Inputs to the neural network
    double output; // Output (the next number to predict)
};

// Function to generate a random sequence of numbers
std::vector<double> generateRandomSequence(int length, double min_value, double max_value) {
    std::vector<double> sequence;
    std::default_random_engine generator(static_cast<unsigned int>(std::time(0)));
    std::uniform_real_distribution<double> distribution(min_value, max_value);

    for (int i = 0; i < length; ++i) {
        sequence.push_back(distribution(generator));
    }

    return sequence;
}

// Create training data using a sliding window
std::vector<DataPoint> createTrainingData(const std::vector<double>& sequence, int window_size) {
    std::vector<DataPoint> training_data;

    for (size_t i = 0; i < sequence.size() - window_size; ++i) {
        DataPoint data_point;
        data_point.inputs = std::vector<double>(sequence.begin() + i, sequence.begin() + i + window_size);
        data_point.output = sequence[i + window_size]; // The next number to predict
        training_data.push_back(data_point);
    }

    return training_data;
}

// Neural network structure with hidden layers
struct NeuralNetwork {
    int input_size;
    std::vector<int> hidden_sizes;
    int output_size;

    std::vector<std::vector<std::vector<double>>> weights; // 3D vector for weights
    std::vector<std::vector<double>> layers; // All layers, including hidden and output layers

    NeuralNetwork(int input, std::vector<int> hidden, int output)
        : input_size(input), hidden_sizes(hidden), output_size(output) {
        // Initialize the layers
        layers.push_back(std::vector<double>(input_size)); // Input layer

        // Add hidden layers
        for (int size : hidden_sizes) {
            layers.push_back(std::vector<double>(size));
        }

        // Add output layer
        layers.push_back(std::vector<double>(output_size));

        // Initialize weights
        for (size_t i = 0; i < layers.size() - 1; ++i) {
            init_weights(layers[i].size(), layers[i + 1].size());
        }
    }

    // Initialize weights with random values
    void init_weights(int from, int to) {
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(-1.0, 1.0);

        std::vector<std::vector<double>> weight_matrix(from, std::vector<double>(to));
        for (int i = 0; i < from; ++i) {
            for (int j = 0; j < to; ++j) {
                weight_matrix[i][j] = distribution(generator);
            }
        }

        weights.push_back(weight_matrix);
    }

    // Forward pass
    std::vector<double> forward(const std::vector<double>& inputs) {
        assert(inputs.size() == input_size); // Ensure correct input size
        layers[0] = inputs; // Set the first layer as inputs

        for (size_t l = 0; l < weights.size(); ++l) {
            const auto& current_layer = layers[l];
            const auto& current_weights = weights[l];
            auto& next_layer = layers[l + 1];

            for (size_t j = 0; j < next_layer.size(); ++j) {
                double sum = 0;
                for (size_t i = 0; i < current_layer.size(); ++i) {
                    sum += current_layer[i] * current_weights[i][j];
                }

                next_layer[j] = sigmoid(sum); // Apply activation function
            }
        }

        return layers.back(); // Return the output layer
    }

    // Backpropagation
    void backpropagate(const std::vector<double>& inputs, double target, double learning_rate) {
        assert(inputs.size() == input_size); // Ensure correct input size
        forward(inputs); // Perform forward pass to calculate all layers

        // Initialize errors
        std::vector<std::vector<double>> errors(layers.size()); // Proper initialization
        errors.back().resize(output_size); // Resize to output size

        // Calculate output errors
        for (int j = 0; j < output_size; ++j) {
            double output = layers.back()[j];
            errors.back()[j] = (target - output) * sigmoid_derivative(output);
        }

        // Calculate errors for hidden layers
        for (int l = errors.size() - 2; l >= 0; --l) {
            const auto& current_weights = weights[l];
            const auto& next_errors = errors[l + 1];
            auto& hidden_errors = errors[l]; // Ensure correct initialization

            hidden_errors.resize(layers[l].size()); // Ensure proper resizing

            for (int i = 0; i < layers[l].size(); ++i) {
                double sum = 0;
                for (int j = 0; j < next_errors.size(); ++j) { // Ensure valid bounds
                    sum += next_errors[j] * current_weights[i][j];
                }
                hidden_errors[i] = sum * sigmoid_derivative(layers[l][i]);
            }
        }

        // Update weights based on the calculated errors
        for (size_t l = 0; l < weights.size(); ++l) {
            const auto& current_layer = layers[l];
            auto& current_weights = weights[l];
            const auto& hidden_errors = errors[l + 1];

            for (int i = 0; i < current_layer.size(); ++i) {
                for (int j = 0; j < hidden_errors.size(); ++j) {
                    current_weights[i][j] += learning_rate * current_layer[i] * hidden_errors[j]; // Correct bounds
                }
            }
        }
    }
};

// Main function to predict the next number in a random sequence
int main() {
    int input_size = 10; // Window size for input data
    std::vector<int> hidden_sizes = {20, 15, 10}; // Hidden layer configuration
    int output_size = 1; // Single output (the next number in the sequence)

    NeuralNetwork nn(input_size, hidden_sizes, output_size);

    // Generate a random sequence
    int sequence_length = 50; // Sequence length
    std::vector<double> random_sequence = generateRandomSequence(sequence_length, 0.0, 1.0); // Generate random sequence

    // Create training data using a sliding window approach
    auto training_data = createTrainingData(random_sequence, input_size);

    double learning_rate = 0.05; // Learning rate for backpropagation
    int epochs = 1000; // Number of epochs for training

    // Train the neural network
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (const auto& dp : training_data) {
            nn.backpropagate(dp.inputs, dp.output, learning_rate); // Train with backpropagation
        }
    }

    // Predict the next number in the sequence
    std::vector<double> last_inputs(random_sequence.end() - input_size, random_sequence.end()); // Last window
    auto predicted_output = nn.forward(last_inputs); // Get the prediction

    std::cout << "Predicted next number: " << predicted_output[0] << std::endl; // Output the result

    return 0;
}
