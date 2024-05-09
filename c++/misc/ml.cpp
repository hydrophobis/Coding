#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include <cassert>
#include <ctime> // For time-based seed
#include <functional>

// ANSI color escape codes
#define R                   "\x1b[0m"
#define ANSI_COLOR_BLACK    "\x1b[30m"
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_WHITE    "\x1b[37m"

// Bright versions of colors
#define ANSI_COLOR_BRIGHT_BLACK    "\x1b[30;1m"
#define ANSI_COLOR_BRIGHT_RED      "\x1b[31;1m"
#define ANSI_COLOR_BRIGHT_GREEN    "\x1b[32;1m"
#define ANSI_COLOR_BRIGHT_YELLOW   "\x1b[33;1m"
#define ANSI_COLOR_BRIGHT_BLUE     "\x1b[34;1m"
#define BM                         "\x1b[35;1m"
#define ANSI_COLOR_BRIGHT_CYAN     "\x1b[36;1m"
#define ANSI_COLOR_BRIGHT_WHITE    "\x1b[37;1m"

// Sigmoid activation function
double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

// Derivative of sigmoid function (for backpropagation)
double sigmoid_derivative(double x) {
    return sigmoid(x) * (1 - sigmoid(x));
}

// Structure for a single training data point
struct DataPoint {
    std::vector<double> inputs;
    std::vector<double> outputs;
};

// Function to generate random training data with time-based seed
std::vector<DataPoint> generateRandomTrainingData(int num_samples, int num_inputs, int num_outputs, double min_value, double max_value) {
    std::vector<DataPoint> training_data;

    // Seed the random number generator with the current time
    std::default_random_engine generator(static_cast<unsigned int>(std::time(0)));
    std::uniform_real_distribution<double> distribution(min_value, max_value);

    for (int i = 0; i < num_samples; ++i) {
        DataPoint data_point;

        // Generate random input features
        for (int j = 0; j < num_inputs; ++j) {
            data_point.inputs.push_back(distribution(generator));
        }

        // Generate random target outputs
        for (int k = 0; k < num_outputs; ++k) {
            data_point.outputs.push_back(distribution(generator));
        }

        training_data.push_back(data_point);
    }

    return training_data;
}

// Neural network with multiple hidden layers
struct NeuralNetwork {
    int input_size;
    std::vector<int> hidden_sizes;
    int output_size;

    std::vector<std::vector<std::vector<double>>> weights; // 3D vector for weights between layers
    std::vector<std::vector<double>> layers; // All layers, including hidden and output layers

    // Constructor to initialize the neural network
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

        // Initialize the weights
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
        assert(inputs.size() == input_size);
        layers[0] = inputs;

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
    void backpropagate(const std::vector<double>& inputs, const std::vector<double>& targets, double learning_rate) {
        assert(inputs.size() == input_size);
        assert(targets.size() == output_size);

        forward(inputs); // Perform forward pass to calculate all layers

        // Calculate output errors
        std::vector<std::vector<double>> errors(layers.size());
        errors.back().resize(output_size);

        for (int j = 0; j < output_size; ++j) {
            double output = layers.back()[j];
            errors.back()[j] = (targets[j] - output) * sigmoid_derivative(output);
        }

        for (int l = errors.size() - 2; l >= 0; --l) {
            const auto& next_weights = weights[l];
            const auto& next_errors = errors[l + 1];
            auto& current_errors = errors[l];

            current_errors.resize(layers[l].size());

            for (int i = 0; i < layers[l].size(); ++i) {
                double sum = 0;
                const auto& current_weights = weights[l]; // Declare current_weights here
                for (int j = 0; j < layers[l + 1].size(); ++j) {
                    sum += next_errors[j] * current_weights[i][j];
                }
                current_errors[i] = sum * sigmoid_derivative(layers[l][i]);
            }
        }


        // Update weights based on the calculated errors
        for (size_t l = 0; l < weights.size(); ++l) {
            const auto& current_layer = layers[l];
            auto& current_weights = weights[l];
            const auto& current_errors = errors[l + 1];

            for (int i = 0; i < current_layer.size(); ++i) {
                for (int j = 0; j < current_weights[i].size(); ++j) {
                    current_weights[i][j] += learning_rate * current_layer[i] * current_errors[j];
                }
            }
        }
    }
};

int main() {
    int input_size = 12; // Amount of fed inputs
    std::vector<int> hidden_sizes = {350, 250, 200, 175, 150}; // Hidden layer configuration
    int output_size =  5; // Output size
    int num_samples = 3; // Number of training samples
    double min_value = 0.0; // Minimum value for random generation
    double max_value = 1.0; // Maximum value for random generation
    double tolerance = 0.005; // Define an acceptable margin for correct predictions
    int epochs = 1000; // Number of epochs
    double learning_rate = 0.05; // Learning rate

    NeuralNetwork nn(input_size, hidden_sizes, output_size);

    auto training_data = generateRandomTrainingData(num_samples, input_size, output_size, min_value, max_value);


    // Train the neural network
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (const auto& dp : training_data) {
            nn.forward(dp.inputs);
            nn.backpropagate(dp.inputs, dp.outputs, learning_rate);
        }
    }

    // Calculate absolute differences between predicted and expected outputs
    double total_absolute_difference = 0.0;
    for (const auto& dp : training_data) {
        auto output = nn.forward(dp.inputs);

        double sample_absolute_difference = 0.0;
        for (int i = 0; i < output.size(); ++i) {
            sample_absolute_difference += std::abs(output[i] - dp.outputs[i]);
        }
        // Average absolute difference for this sample
        sample_absolute_difference /= output.size();

        total_absolute_difference += sample_absolute_difference;

        std::cout << BM << "Inputs: " << R;
        for (const auto& input : dp.inputs) {
            std::cout << input << " ";
        }

        std::cout << std::endl << BM << "Predicted Outputs: " << R;
        for (const auto& out : output) {
            std::cout << out << " ";
        }

        std::cout << std::endl << BM << "Expected Outputs: " << R;
        for (const auto& out : dp.outputs) {
            std::cout << out << " ";
        }

        std::cout << std::endl << BM << "Absolute Difference: " << R << sample_absolute_difference << std::endl << R;
    }

    // Average absolute difference across all samples
    double average_absolute_difference = total_absolute_difference / training_data.size();

    // Accuracy (inversed error)
    double accuracy = 100.0 - (average_absolute_difference / (max_value - min_value)) * 100.0;

    std::cout << BM << "Accuracy: " << R << accuracy << "%" << std::endl;

    return 0;
}
