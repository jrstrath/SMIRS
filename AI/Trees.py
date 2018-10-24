#import the necessary libraries
from sklearn import preprocessing
import pandas as pd
import numpy as np

#Decision Tree Import
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score
from sklearn import tree


#Read the dataframe
Irrigate = pd.read_csv('Datasets/Data.csv')

#Find and replace the values
dictionary = {'Yes': 1, 'No': 0}
Irrigate.replace(dictionary, inplace=True)
X = Irrigate[['Temperature', 'Relative Humidity' , 'Soil moisture']].values
Y = Irrigate['Irrigate?']
X_train, X_test, Y_train, Y_test= train_test_split(X, Y, test_size=0.4, random_state = 2)

#Decision Trees
clf_gini = DecisionTreeClassifier(criterion = "gini", random_state = 100, max_depth=10, min_samples_leaf=10)
clf_gini.fit(X_train, Y_train)


#Prediciting with the model
def Pred(temp, rh, sm):
    prediction = clf_gini.predict([[temp, rh, sm]])
    print(prediction)

Pred(28,65,620);

#Testing
#y_train = clf_gini.predict(X_train)
#y_train
#print ("Training accuracy is ", accuracy_score(Y_train,y_train)*100)

#Training
#y_pred = clf_gini.predict(X_test)
#y_pred
#print ("Testing accuracy is ", accuracy_score(Y_test,y_pred)*100)

