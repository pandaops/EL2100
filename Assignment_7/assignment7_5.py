import matplotlib.pyplot as graph

FILENAME='../Assignment_5/Assignment-5.lyx'

# Form list of worcount
with file(FILENAME,'rb') as txt:
    lines=txt.readlines()
    count=[]
    for line in lines:
        count.extend([len(x) for x in line.split()])
    
# Plot graph    
graph.hist(count,range(20))
graph.show()
