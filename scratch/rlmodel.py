import random
from time import time
import numpy as np
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM, Embedding, Dropout
from keras.optimizers import SGD, Adam
from beautifultable import BeautifulTable
from keras.callbacks import TensorBoard
import matplotlib.pyplot as plt
import pylab

class DQNAgent: 
    def __init__(self, state_size, actions):
        self.state_size = state_size
        self.action_size = actions
        self.gamma = 0.95
        self.epsilon = 0.01
        self.epsilon_min = 0.01
        self.epsilon_decay = 0.995 
        self.learning_rate = 0.001 
        self.memory = [] 
        self.model = self.build_model()

    def build_model(self): 
        self.model = Sequential()
        self.model.add(Embedding(self.state_size, 10))
        self.model.add(Dropout(0.2))
	self.model.add(LSTM(32, activation='relu', return_sequences=True)) 
        self.model.add(LSTM(16, activation='relu', return_sequences=True))
        self.model.add(LSTM(8, activation='relu', return_sequences=True))
        self.model.add(Dense(self.action_size, activation='linear')) 
        sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
        self.model.compile(loss='mse', optimizer=Adam(lr=self.learning_rate), metrics=['accuracy'])	
        return self.model

    def remember(self, state, action, reward, next_state, done): 
        self.memory.append((state, action, reward, next_state, done))

    def act(self, state): 
        if np.random.rand() <= self.epsilon:
            return np.random.randint(1, self.action_size) 	     	    
        act_values = self.model.predict(state) 	
	#return np.amax(act_values)
        #return np.argmax(act_values[0]) 
  
    def q_values(self, state): 
        if np.random.rand() <= self.epsilon:
            return np.random.randint(1, self.action_size) 	     	    
        act_values = self.model.predict(state) 
	return np.mean(act_values[0])
	#return np.amax(act_values[1])
       		
    def replay(self, batch_size): 
        minibatch = random.sample(self.memory, batch_size)
        for state, action, reward, next_state, done in minibatch:
            target = reward
            if not done:
                target = reward + self.gamma * \
                                  np.amax(self.model.predict(next_state)[0])
                target_f = self.model.predict(state) 
                target_f[0][action] = target	
		tensorboard = TensorBoard(log_dir='logs/{}'.format(time()),batch_size=batch_size,write_images=True,embeddings_freq=10)
                history = self.model.fit(state, target_f, epochs=500, verbose=0,callbacks=[tensorboard])		
                #history = self.model.fit(state, target_f, validation_split=0.33, epochs=50, verbose=0)
				
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay
	#print(self.model.summary())

    	'''plt.plot(history.history['acc'])	
    	plt.title('model accuracy')
    	plt.ylabel('accuracy')
    	plt.xlabel('epoch')
    	plt.legend(['train', 'test'], loc='upper left')
    	plt.show()

	plt.plot(history.history['loss'])	
    	plt.title('model loss')
    	plt.ylabel('loss')
    	plt.xlabel('epoch')
    	plt.legend(['train', 'test'], loc='upper left')
    	plt.show()'''


 
