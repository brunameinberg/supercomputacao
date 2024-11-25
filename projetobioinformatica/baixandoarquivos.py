import os
import subprocess
from urllib.request import urlretrieve

# Definir o diretório de destino
output_dir = "dados"
os.makedirs(output_dir, exist_ok=True)

# Base URL dos arquivos
base_url = 'ftp://hgdownload.cse.ucsc.edu/goldenPath/hg19/snp147Mask'

# Loop pelos arquivos de chr1 a chr22
for i in range(1, 23):
    file_name = f'chr{i}.subst.fa.gz'
    file_url = f'{base_url}/{file_name}'
    destination = os.path.join(output_dir, file_name)
    
    print(f"Baixando {file_name}...")
    # Baixar o arquivo
    urlretrieve(file_url, destination)
    
    print(f"Descompactando {file_name}...")
    # Descompactar o arquivo
    subprocess.run(["gunzip", destination])
    
print("Download e descompactação concluídos.")
