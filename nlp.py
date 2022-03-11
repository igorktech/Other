import nltk
from nltk.tokenize import sent_tokenize
from nltk.tokenize import word_tokenize

import pandas as pd

loc = ("..\\Downloads\\test.xlsx")
 


# Load the xlsx file
excel_data = pd.read_excel(loc)
# Read the values of the file in the dataframe
data = pd.DataFrame(excel_data, columns=['date', 'text'])

for string in excel_data.text:
    tokenized_words = word_tokenize(string, language = 'russian')
    tagged_words = nltk.pos_tag(tokenized_words)
    # grammar  = "NP : {<DT>?<JJ>*<NN>}"
    # #r""" NP: {<.*>+}}<JJ>+{"""

    # parser  = nltk.RegexpParser(grammar)

    # output = parser.parse(tagged_words)
    # print(output)
    # output.draw()
    N_E_R = nltk.ne_chunk(tagged_words, binary = False)
    print(N_E_R)
    N_E_R.draw()
        

