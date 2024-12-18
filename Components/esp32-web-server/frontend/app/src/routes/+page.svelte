<script>
  const ESP32_IP = "192.168.238.49";
  let isFetching = false;

  async function analogRead(pin) {
    if (isFetching) return;
    isFetching = true;

    const timeout = setTimeout(() => {
      isFetching = false;
    }, 1000);

    const res = await fetch(`http://${ESP32_IP}/analogRead/${pin}`);
    const text = await res.text();
    clearTimeout(timeout);
    isFetching = false;
    return Number(text);
  }

  async function analogWrite(pin, value) {
    if (isFetching) return;
    isFetching = true;

    const timeout = setTimeout(() => {
      isFetching = false;
    }, 1000);

    const res = await fetch(`http://${ESP32_IP}/analogWrite/${pin}/${value}`);
    
    clearTimeout(timeout);
    isFetching = false;
  }

  function map(value, inMin, inMax, outMin, outMax) {
    return Math.round(
      ((value - inMin) * (outMax - outMin)) / (inMax - inMin) + outMin,
    );
  }

  let potenziometroValue = 0;

  setInterval(async () => {
    try {
      const valorePotenziometro = await analogRead(32);
      if(valorePotenziometro) potenziometroValue = valorePotenziometro;
      await analogWrite(2, map(potenziometroValue, 0, 2 ** 12, 0, 255));
    } catch (err) {
      console.error(err);
    }
  }, 100);
</script>

<main class="p-4 w-full grid bg-slate-900">
  <input
    type="range"
    min={0}
    max={2 ** 12}
    value={potenziometroValue}
    disabled
  />
  <p class="text-white text-3xl">{potenziometroValue}</p>
</main>

<style>
  :global(body, main, html) {
    @apply h-full;
  }
</style>
