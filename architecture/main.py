import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import datetime, timedelta

# Определяем этапы лабораторной работы и их продолжительность
tasks = [
    ("Изучение теории", "2025-01-21", "2025-01-23"),
    ("Разработка кода", "2025-01-23", "2025-02-27"),
    ("Тестирование и отладка", "2025-02-27", "2025-03-02"),
    ("Написание отчёта", "2025-03-02", "2025-03-04"),
    ("Подготовка к сдаче", "2025-03-04", "2025-03-05"),
    ("Сдача", "2025-03-06", "2025-03-07")
]
tasks.reverse()

fig, ax = plt.subplots(figsize=(10, 6))

# Преобразуем даты в формат datetime
start_dates = [datetime.strptime(task[1], "%Y-%m-%d") for task in tasks]
end_dates = [datetime.strptime(task[2], "%Y-%m-%d") for task in tasks]
durations = [(end - start).days for start, end in zip(start_dates, end_dates)]

# Строим диаграмму Ганта
for i, (task, start, duration) in enumerate(zip(tasks, start_dates, durations)):
    ax.barh(task[0], duration, left=start, color="skyblue")

# Настройка осей
ax.set_xlabel("Дата")
ax.set_ylabel("Этапы выполнения 3 лабораторной работы по системному ПО")
ax.set_title("Диаграмма Ганта для лабораторной работы №3")
ax.xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d"))
ax.xaxis.set_major_locator(mdates.DayLocator(interval=2))
plt.xticks(rotation=45)
plt.grid(axis="x", linestyle="--", alpha=0.7)

plt.show()
