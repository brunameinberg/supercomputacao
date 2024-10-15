import numpy as np
import pandas as pd

# Simulação dos tempos de execução (em segundos) para diferentes schedulers com 3 execuções por teste
schedulers = [
    "default", "static", "static, 4", "static, 8", "dynamic", "dynamic, 1", 
    "dynamic, 4", "dynamic, 8", "guided", "guided, 2", "guided, 4", "guided, 8", "auto", "runtime"
]

# Tempos simulados de execução (3 execuções por scheduler)
execution_times = np.random.uniform(0.1, 0.5, (len(schedulers), 3))

# Calcula o tempo médio para cada scheduler
mean_times = np.mean(execution_times, axis=1)

# Cria o DataFrame para mostrar os resultados
df_results = pd.DataFrame({
    "Scheduler": schedulers,
    "Execução 1 (s)": execution_times[:, 0],
    "Execução 2 (s)": execution_times[:, 1],
    "Execução 3 (s)": execution_times[:, 2],
    "Tempo Médio (s)": mean_times
})

# Exibe os resultados
import ace_tools as tools; tools.display_dataframe_to_user(name="Scheduler Execution Times", dataframe=df_results)
