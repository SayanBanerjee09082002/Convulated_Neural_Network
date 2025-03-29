#include "layers.h"

// Activation functions
float relu(float x)
{
    return x > 0 ? x : 0.0;
}

float relu_derivative(float x)
{
    return x > 0 ? 1 : 0.0;
}

float sigmoid(float x)
{
    return 1.0f / (1.0f + exp(-x));
}

float sigmoid_derivative(float x)
{
    return x * (1.0f - x);
}

Neural_Network::Neural_Network(int input_size, int hidden_size,
                               int output_size, float learning_rate)
    : input_size(input_size), hidden_size(hidden_size),
      output_size(output_size), learning_rate(learning_rate)
{
    // Initialization for sigmoid activation
    std::default_random_engine generator;
    generator.seed(std::time(0));
    float stddev = sqrt(2.0f / input_size);
    std::normal_distribution<float> dist(0.0, stddev);

    // Initialize weights and biases
    weights1.resize(hidden_size, std::vector<float>(input_size));
    for (auto &row : weights1)
        for (auto &val : row)
            val = dist(generator);

    stddev = sqrt(2.0f / hidden_size);
    dist = std::normal_distribution<float>(0.0, stddev);

    weights2.resize(output_size, std::vector<float>(hidden_size));
    for (auto &row : weights2)
        for (auto &val : row)
            val = dist(generator);

    bias1.resize(hidden_size, 0.0);
    bias2.resize(output_size, 0.0);
}

Neural_Network::~Neural_Network() {}

std::vector<float> Neural_Network::forward_pass(
    const std::vector<float> &input,
    std::vector<float> &hidden_out)
{
    // Hidden layer computation
    hidden_out.resize(hidden_size);
    for (int i = 0; i < hidden_size; i++)
    {
        float sum = bias1[i];
        for (int j = 0; j < input_size; j++)
            sum += input[j] * weights1[i][j];
        hidden_out[i] = sigmoid(sum); // Sigmoid activation
    }

    // Output layer computation
    std::vector<float> output(output_size);
    for (int i = 0; i < output_size; i++)
    {
        float sum = bias2[i];
        for (int j = 0; j < hidden_size; j++)
            sum += hidden_out[j] * weights2[i][j];
        output[i] = sigmoid(sum); // Sigmoid activation
    }
    return output;
}

void Neural_Network::backward_pass(
    const std::vector<float> &input,
    const std::vector<float> &hidden,
    const std::vector<float> &output,
    int target)
{
    // Create one-hot encoded target
    std::vector<float> target_onehot(output_size, 0.0);
    target_onehot[target] = 1.0f;

    // Calculate output layer error
    std::vector<float> delta_output(output_size);
    for (int i = 0; i < output_size; i++)
        delta_output[i] = (output[i] - target_onehot[i]) * sigmoid_derivative(output[i]);

    // Calculate hidden layer error
    std::vector<float> delta_hidden(hidden_size);
    for (int j = 0; j < hidden_size; j++)
    {
        float error = 0.0f;
        for (int i = 0; i < output_size; i++)
            error += delta_output[i] * weights2[i][j];
        delta_hidden[j] = error * sigmoid_derivative(hidden[j]);
    }

    // Update output layer weights
    for (int i = 0; i < output_size; i++)
    {
        for (int j = 0; j < hidden_size; j++)
            weights2[i][j] -= learning_rate * delta_output[i] * hidden[j];
        bias2[i] -= learning_rate * delta_output[i];
    }

    // Update hidden layer weights
    for (int i = 0; i < hidden_size; i++)
    {
        for (int j = 0; j < input_size; j++)
            weights1[i][j] -= learning_rate * delta_hidden[i] * input[j];
        bias1[i] -= learning_rate * delta_hidden[i];
    }
}

void Neural_Network::train(const std::vector<float> &input, int label)
{
    std::vector<float> hidden_activations;
    auto output_probs = forward_pass(input, hidden_activations);
    backward_pass(input, hidden_activations, output_probs, label);
}

int Neural_Network::predict(const std::vector<float> &input)
{
    std::vector<float> hidden_activations;
    auto output_probs = forward_pass(input, hidden_activations);
    return std::max_element(output_probs.begin(),
                            output_probs.end()) -
           output_probs.begin();
}