(function () {
  const SERVER = 'http://localhost:8080';

  const expressionEl = document.getElementById('expression');
  const resultEl = document.getElementById('result');
  const screenEl = document.getElementById('screen');
  const padEl = document.getElementById('pad');
  const statusEl = document.getElementById('status');

  let current = '0';
  let previous = null;
  let pendingOp = null;
  let justEvaluated = false;
  let startFresh = false;
  let expressionText = '';
  let serverOnline = false;

  function render() {
    resultEl.textContent = current;
    expressionEl.textContent = expressionText || '\u00A0';
  }

  function showError(message) {
    screenEl.classList.add('is-error');
    current = message;
    render();
    justEvaluated = true;
  }

  function clearError() {
    screenEl.classList.remove('is-error');
  }

  async function checkServer() {
    try {
      const res = await fetch(SERVER + '/add?a=1&b=1', { cache: 'no-store' });
      const data = await res.json();
      serverOnline = res.ok && data.ok === true;
    } catch (e) {
      serverOnline = false;
    }
    statusEl.textContent = serverOnline
      ? 'C++ server online'
      : 'C++ server offline';
    statusEl.className = 'status ' + (serverOnline ? 'online' : 'offline');
    padEl.querySelectorAll('button:not([data-action="reconnect"])').forEach(b => {
      b.disabled = !serverOnline;
    });
  }

  async function callServer(path, params) {
    const qs = Object.entries(params).map(([k, v]) => k + '=' + encodeURIComponent(v)).join('&');
    const res = await fetch(SERVER + '/' + path + '?' + qs, { cache: 'no-store' });
    const data = await res.json();
    if (!data.ok) throw new Error(data.error || 'Error');
    return data.result;
  }

  function clearAll() {
    current = '0';
    previous = null;
    pendingOp = null;
    justEvaluated = false;
    startFresh = false;
    expressionText = '';
    clearError();
    render();
  }

  function inputDigit(d) {
    if (justEvaluated) { current = '0'; expressionText = ''; justEvaluated = false; clearError(); }
    if (startFresh) { current = '0'; startFresh = false; }
    if (d === '.') {
      if (current.includes('.')) return;
      current = current === '0' ? '0.' : current + '.';
    } else {
      current = current === '0' ? d : current + d;
    }
    render();
  }

  function opEndpoint(op) {
    return { '+': 'add', '-': 'subtract', 'x': 'multiply', '/': 'divide', '%': 'modulo' }[op];
  }

  async function chooseOperator(op) {
    const val = parseFloat(current);
    if (pendingOp && previous !== null && !justEvaluated) {
      try {
        const r = await callServer(opEndpoint(pendingOp), { a: previous, b: val });
        previous = r;
        current = String(r);
      } catch (e) {
        showError(e.message);
        return;
      }
    } else {
      previous = val;
    }
    pendingOp = op;
    justEvaluated = false;
    startFresh = true;
    expressionText = previous + ' ' + op;
    render();
  }

  async function equals() {
    if (pendingOp === null || previous === null) return;
    const val = parseFloat(current);
    try {
      const r = await callServer(opEndpoint(pendingOp), { a: previous, b: val });
      expressionText = previous + ' ' + pendingOp + ' ' + val + ' =';
      current = String(r);
      previous = null;
      pendingOp = null;
      justEvaluated = true;
      clearError();
      render();
    } catch (e) {
      showError(e.message);
    }
  }

  async function unary(path, label) {
    const val = parseFloat(current);
    try {
      const r = await callServer(path, { a: val });
      expressionText = label + '(' + val + ') =';
      current = String(r);
      justEvaluated = true;
      clearError();
      render();
    } catch (e) {
      showError(e.message);
    }
  }

  async function handleAction(action) {
    if (action === 'clear') { clearAll(); return; }
    if (action === 'reconnect') { await checkServer(); return; }
    if (!serverOnline) return;

    switch (action) {
      case 'add': await chooseOperator('+'); break;
      case 'subtract': await chooseOperator('-'); break;
      case 'multiply': await chooseOperator('x'); break;
      case 'divide': await chooseOperator('/'); break;
      case 'percent': await chooseOperator('%'); break;
      case 'equals': await equals(); break;
      case 'log': await unary('log', 'log'); break;
      case 'square': await unary('square', 'sq'); break;
      case 'cube': await unary('cube', 'cb'); break;
      case 'sin': await unary('sin', 'sin'); break;
      case 'cos': await unary('cos', 'cos'); break;
      case 'cot': await unary('cot', 'cot'); break;
    }
  }

  padEl.addEventListener('click', (e) => {
    const btn = e.target.closest('button');
    if (!btn) return;
    if (btn.dataset.num !== undefined) {
      if (!serverOnline) return;
      inputDigit(btn.dataset.num);
      return;
    }
    if (btn.dataset.action) handleAction(btn.dataset.action);
  });

  clearAll();
  checkServer();
  setInterval(checkServer, 5000);
})();
