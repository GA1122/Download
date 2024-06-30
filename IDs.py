import pandas as pd
import csv

edges = r"/home/gas690/Download/code_gnn/storage/processed/bigvul/edges.csv"
nodes = r"/home/gas690/Download/code_gnn/storage/processed/bigvul/nodes.csv"
database = r"/home/gas690/Download/code_gnn/storage/external/database.csv"
dataset = r"/home/gas690/Download/code_gnn/storage/external/MSR_data_cleaned.csv"

edgesDF = pd.read_csv(edges)
nodesDF = pd.read_csv(nodes)
datasetDF = pd.read_csv(dataset)

with open(database, 'w', newline='\n') as file:
    writer = csv.writer(file)
    writer.writerow(["ID", "num_nodes", "num_edges", "vuln"])
    num_nodes = 0
    num_edges = 0
    vuln = 0
    for id in datasetDF["Unnamed: 0"].unique():
        num_nodes = nodesDF["graph_id"].value_counts()[id]
        num_edges = edgesDF["graph_id"].value_counts()[id]
        vuln = datasetDF.loc[datasetDF["Unnamed: 0"] == id, "vul"].iloc[0]
        writer.writerow([id, num_nodes, num_edges, vuln])
    file.close()
    
