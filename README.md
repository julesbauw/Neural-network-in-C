# Neural Network in C



## compile

Enter this command in the ./src directory:

```
gcc -g $(cat ../sources) -O3 -march=native -lm -o main
```

## run

```
./main [train|test] neural_network_file data_file
```

If **train**, a new neural network wil be created in the file "neural_network_file" and will be trained on data in the "data_file"

if **test**, the neural network in file "neural_network_file" will be loaded and tested on the data in "data_file"

## data set

```
kaggle datasets download -d oddrationale/mnist-in-csv
```



| File | Hidden nodes | Activation function | Loss function | Epochs | Result |
|------|--------------|---------------------|---------------|--------|--------|
| nn1  | 500          | Sigmoid             | Half MSE      | 20     | 90.12% |
| nn2  | 128          | Sigmoid             | Half MSE      | 20     | 90.17% |
| nn3  | 128          | Sigmoid             | Half MSE      | 40     | 91.23% |
| nn4  | 64           | Sigmoid             | Half MSE      | 100    | 93.10% |




