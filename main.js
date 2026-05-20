/*
  main.js — Bloomhaven Care (Warm Futurism Rebuild)
  ─────────────────────────────────────────────────────
  [CONFIG]     — Contact email
  [DATA]       — Services, team, skills, FAQs
  [PARTICLES]  — Hero canvas particle system
  [COUNTERS]   — Animated stat counters
  [RENDER]     — Build HTML from data
  [FILTER]     — Service card filtering
  [MODAL]      — Service detail popup
  [FAQ]        — Accordion + live search
  [FORM]       — Contact form (FormSubmit.co)
  [APPLY]      — Quick apply form (recruit page)
*/

/* ════════════════════════════════════════════
   [CONFIG]
   ════════════════════════════════════════════ */
const CONTACT_EMAIL = 'Info@bloomhavencare.co.uk';
const FORM_CONFIG = {
  subject: 'New Enquiry — Bloomhaven Care',
  cc: '',
};

const SERVICE_IMAGES = [
  {
    src: 'https://images.unsplash.com/photo-1666887360680-9dc27a1d2753?auto=format&fit=crop&fm=jpg&ixid=M3wxMjA3fDB8MHxzZWFyY2h8Mnx8aG9tZSUyMGhlYWx0aCUyMGNhcmV8ZW58MHx8MHx8fDA%3D&ixlib=rb-4.1.0&q=72&w=900',
    alt: 'Care professional checking blood pressure during a home visit'
  },
  {
    src: 'https://images.unsplash.com/photo-1723433892471-62f113c8c9a0?auto=format&fit=crop&fm=jpg&ixid=M3wxMjA3fDB8MHxzZWFyY2h8M3x8aG9tZSUyMGhlYWx0aCUyMGNhcmV8ZW58MHx8MHx8fDA%3D&ixlib=rb-4.1.0&q=72&w=900',
    alt: 'Care worker supporting an older person using a wheelchair'
  },
  {
    src: 'https://images.unsplash.com/photo-1765896387377-e293914d1e69?auto=format&fit=crop&fm=jpg&ixid=M3wxMjA3fDB8MHxzZWFyY2h8MjB8fGhvbWUlMjBoZWFsdGglMjBjYXJlfGVufDB8fDB8fHww&ixlib=rb-4.1.0&q=72&w=900',
    alt: 'Care worker and older person laughing together indoors'
  },
  {
    src: 'https://images.unsplash.com/photo-1666887360726-f55472d96c34?auto=format&fit=crop&fm=jpg&ixid=M3wxMjA3fDB8MHxzZWFyY2h8Nnx8aG9tZSUyMGhlYWx0aCUyMGNhcmV8ZW58MHx8MHx8fDA%3D&ixlib=rb-4.1.0&q=72&w=900',
    alt: 'Care worker helping an older person use a tablet'
  },
  {
    src: 'https://images.unsplash.com/photo-1658632302217-984d432b4d38?auto=format&fit=crop&fm=jpg&ixid=M3wxMjA3fDB8MHxzZWFyY2h8OHx8aG9tZSUyMGhlYWx0aCUyMGNhcmV8ZW58MHx8MHx8fDA%3D&ixlib=rb-4.1.0&q=72&w=900',
    alt: 'Care worker speaking with an older person in a living room'
  },
  {
    src: 'https://images.unsplash.com/photo-1758691462477-976f771224d8?auto=format&fit=crop&fm=jpg&ixid=M3wxMjA3fDB8MHxzZWFyY2h8MTl8fGhvbWUlMjBoZWFsdGglMjBjYXJlfGVufDB8fDB8fHww&ixlib=rb-4.1.0&q=72&w=900',
    alt: 'Home care consultation with an older person on a couch'
  }
];

/* ════════════════════════════════════════════
   [DATA] — SERVICES
   ════════════════════════════════════════════ */
const services = [
  {
    title: 'Personal Care',
    icon: '🛁',
    tag: 'Daily Living',
    color: 'linear-gradient(135deg,#1B5E3B,#2E7D52)',
    summary: 'Dignified, sensitive support with washing, dressing, and personal hygiene — delivered with respect and warmth.',
    description: 'Our personal care service provides respectful, sensitive support with all aspects of personal hygiene and daily grooming. Every carer is trained to uphold the dignity and privacy of each client throughout every visit.',
    includes: ['Bathing and showering assistance','Hair washing and styling','Dressing and undressing','Oral hygiene and dental care','Skincare and moisturising','Toileting and continence support','Shaving and grooming','Morning and evening routines']
  },
  {
    title: 'Medication Support',
    icon: '💊',
    tag: 'Health',
    color: 'linear-gradient(135deg,#2E7D52,#4CAF7D)',
    summary: 'Safe, accurate medication prompting and administration by trained carers — giving families total peace of mind.',
    description: 'Our trained carers provide reliable support to ensure medications are taken correctly, at the right time, every day. We liaise with pharmacies and GPs to ensure your loved one\'s health is always in safe hands.',
    includes: ['Medication reminders and prompting','Administration of prescribed medicines','Ordering repeat prescriptions','Liaising with pharmacies and GPs','Medication record keeping','Monitoring for side effects','Safe medication storage checks']
  },
  {
    title: 'Companionship',
    icon: '❤️',
    tag: 'Wellbeing',
    color: 'linear-gradient(135deg,#D4AF37,#E8CC6A)',
    summary: 'Meaningful social connection and emotional support to combat loneliness and enrich everyday life.',
    description: 'Our companionship service goes far beyond simply being present. Our carers build genuine, lasting relationships — sharing interests, conversations, laughter, and life experiences with the people they support.',
    includes: ['Friendly conversation and social interaction','Accompanying to appointments and outings','Help with hobbies and interests','Reading, puzzles and activities','Support attending community events','Technology help (video calls with family)','Light emotional support and reassurance']
  },
  {
    title: 'Meal Preparation',
    icon: '🍽️',
    tag: 'Nutrition',
    color: 'linear-gradient(135deg,#1B5E3B,#D4AF37)',
    summary: 'Nutritious, home-cooked meals tailored to dietary needs, cultural preferences, and medical guidelines.',
    description: 'Our carers prepare delicious, nutritious meals following individual dietary requirements, cultural preferences, and medical guidelines — ensuring every client is well nourished and enjoys mealtimes.',
    includes: ['Breakfast, lunch and dinner preparation','Tailored menus for dietary needs','Grocery shopping assistance','Monitoring food and fluid intake','Special diet support (diabetic, puréed, etc.)','Kitchen tidying and washing up','Batch cooking and meal planning']
  },
  {
    title: 'Housekeeping',
    icon: '🧹',
    tag: 'Home',
    color: 'linear-gradient(135deg,#4CAF7D,#2E7D52)',
    summary: 'Light domestic support to keep the home clean, safe, and comfortable — a peaceful sanctuary.',
    description: 'Our housekeeping service covers essential domestic tasks that keep a home comfortable and safe. This allows clients to focus on enjoying their day and the things they love, not household chores.',
    includes: ['Vacuuming and sweeping floors','Dusting and surface cleaning','Bathroom and kitchen cleaning','Laundry and ironing','Changing bed linen','Taking out bins','Light garden tidying']
  },
  {
    title: 'Dementia Care',
    icon: '🧠',
    tag: 'Specialist',
    color: 'linear-gradient(135deg,#0D2418,#1B5E3B)',
    summary: 'Specialist, compassionate support for people living with dementia — patient, consistent, and deeply caring.',
    description: 'Our dementia-trained carers use person-centred approaches to support clients at all stages of the condition. We work to maintain familiar routines, stimulate positive memories, and provide a calm, reassuring presence.',
    includes: ['Specialist dementia care training','Consistent carer assignment','Routine and structure maintenance','Reminiscence therapy activities','Behavioural and emotional support','Family communication and guidance','Safe home environment support']
  },
  {
    title: 'Live-In Care',
    icon: '🏡',
    tag: '24/7 Support',
    color: 'linear-gradient(135deg,#2E7D52,#0D2418)',
    summary: 'Round-the-clock care from a dedicated live-in carer — a compassionate alternative to a residential care home.',
    description: 'Live-in care allows individuals to remain in their own beloved home with round-the-clock support from a dedicated, trained carer. This is often the preferred alternative to moving into a care home — maintaining independence, familiarity, and comfort.',
    includes: ['24-hour care and companionship','Sleep-in or waking nights available','All daily living tasks covered','Specialist care as required','Regular carer rotations for continuity','Full family communication and updates','Emergency response at any hour']
  },
  {
    title: 'Mobility Support',
    icon: '🦽',
    tag: 'Physical Support',
    color: 'linear-gradient(135deg,#1B5E3B,#4CAF7D)',
    summary: 'Safe, confident assistance with movement, transfers, and independence in and around the home.',
    description: 'Our carers receive full manual handling training and work in partnership with physiotherapists and occupational therapists to support safe movement. We help clients move with confidence and dignity.',
    includes: ['Safe transfers (bed, chair, toilet)','Walking and mobility aid support','Exercise and rehabilitation support','Falls prevention and awareness','Hoist and equipment use','Physiotherapy exercise encouragement','Confidence building for independence']
  },
  {
    title: 'Night Care',
    icon: '🌙',
    tag: 'Overnight',
    color: 'linear-gradient(135deg,#0D2418,#2E7D52)',
    summary: 'Reassuring overnight support so clients — and families — can rest with peace of mind.',
    description: 'We offer both sleep-in and waking night care. Our night carers provide a calm, reassuring presence throughout the night and respond immediately to any needs — giving families the rest they deserve.',
    includes: ['Sleep-in carer (available if needed)','Waking night carer (awake throughout)','Toileting and continence support','Medication administration','Repositioning and pressure care','Reassurance for anxiety or dementia','Emergency response readiness']
  }
];

/* ════════════════════════════════════════════
   [DATA] — TEAM
   ════════════════════════════════════════════ */
const team = [
  {
    name: 'Founding Leadership',
    role: 'Mission & Service Direction',
    emoji: '💚',
    bio: 'Sets the standard for Bloomhaven Care: practical support delivered with kindness, dignity, and careful attention to each person\'s daily life.',
    quals: ['Person-centred care','Service leadership','Family-first values']
  },
  {
    name: 'Registered Care Management',
    role: 'Care Operations & Quality',
    emoji: '📋',
    bio: 'Oversees day-to-day care delivery, care planning, supervision, and quality checks so every visit is safe, respectful, and well coordinated.',
    quals: ['Care planning','Quality assurance','Safeguarding oversight']
  },
  {
    name: 'Care Quality Team',
    role: 'Standards & Continuous Improvement',
    emoji: '⭐',
    bio: 'Reviews feedback, supports carers in the field, and keeps standards moving in the right direction through regular checks and practical coaching.',
    quals: ['Care reviews','Carer supervision','Continuous improvement']
  },
  {
    name: 'Operations Support',
    role: 'Scheduling & Service Coordination',
    emoji: '🗓️',
    bio: 'Keeps visits organised, communication clear, and carers supported so clients and families know what to expect and who to contact.',
    quals: ['Visit scheduling','Family communication','Responsive support']
  },
  {
    name: 'Care Coordination Team',
    role: 'Assessments & Care Plans',
    emoji: '🤝',
    bio: 'Listens carefully during assessments, builds personalised care plans, and keeps those plans updated as needs, routines, and preferences change.',
    quals: ['Home assessments','Personalised plans','Review meetings']
  },
  {
    name: 'Carer Development',
    role: 'Training & Support',
    emoji: '📚',
    bio: 'Supports carers from induction onward with practical guidance, refreshers, and a culture where asking for help is part of doing the job well.',
    quals: ['Induction support','Skills refreshers','Reflective practice']
  }
];

/* ════════════════════════════════════════════
   [DATA] — SKILLS (recruit page)
   ════════════════════════════════════════════ */
const skills = [
  { title: 'Compassion & Empathy',         desc: 'The ability to genuinely understand and share the feelings of those you care for. The NHS identifies this as the single most important quality in a care worker.' },
  { title: 'Communication Skills',          desc: 'Clear, kind, and effective communication with clients, families, and healthcare professionals — both verbal and written.' },
  { title: 'Patience & Resilience',         desc: 'Remaining calm, positive, and effective even in challenging situations — essential for delivering consistent, high-quality care.' },
  { title: 'Reliability & Punctuality',     desc: 'Clients depend on carers arriving when expected. Dependability builds trust and is fundamental to safe, quality care delivery.' },
  { title: 'Attention to Detail',           desc: 'Noticing changes in a client\'s condition, following care plans accurately, and maintaining detailed, accurate records.' },
  { title: 'Respect for Dignity & Privacy', desc: 'Understanding and upholding the rights of individuals to privacy, autonomy, and dignified treatment at all times — a core CQC standard.' },
  { title: 'Physical Stamina & Fitness',    desc: 'Home care can be physically demanding. Good fitness ensures carers can safely support clients with mobility and personal care tasks.' },
  { title: 'Teamwork & Collaboration',      desc: 'Working effectively alongside colleagues, families, GPs, nurses, and allied health professionals to deliver joined-up, coordinated care.' },
  { title: 'Problem Solving',               desc: 'The ability to assess situations quickly, make sound judgements, and respond calmly and effectively in unexpected circumstances.' },
  { title: 'Commitment to Learning',        desc: 'A willingness to engage with training, reflect on practice, and continuously develop knowledge and skills throughout your career.' }
];

/* ════════════════════════════════════════════
   [DATA] — FAQs
   ════════════════════════════════════════════ */
const faqs = [
  {
    q: 'How do I arrange home care for myself or a family member?',
    a: 'Simply contact us via phone, email, or the contact form on this website. One of our friendly care coordinators will arrange a free, no-obligation home assessment at a time that suits you. We\'ll then create a personalised care plan and introduce your dedicated carer.'
  },
  {
    q: 'How much does home care cost?',
    a: 'Costs vary depending on the type of care, number of visits, and duration. Following your free assessment, we\'ll provide a clear, transparent breakdown with no hidden charges. We can also help you explore local authority funding, NHS Continuing Healthcare, and Attendance Allowance.'
  },
  {
    q: 'Can I choose my own carer?',
    a: 'We carefully match clients with carers based on compatibility, needs, and personal preferences. We\'ll always introduce your carer before regular visits begin. If at any point you\'d prefer a different carer, just let us know — your comfort is always our priority.'
  },
  {
    q: 'Are your carers trained and thoroughly vetted?',
    a: 'Absolutely. All Bloomhaven carers undergo enhanced DBS checks, provide multiple references, and complete a comprehensive induction programme before meeting any clients. They also receive ongoing training, regular supervision, and annual appraisals.'
  },
  {
    q: 'What if I need care in an emergency or at short notice?',
    a: 'We do our very best to accommodate urgent care needs. Please contact our office team as soon as possible and we\'ll do everything we can to help. Existing clients can also reach our 24-hour on-call line outside of office hours.'
  },
  {
    q: 'Is Bloomhaven Care regulated by the CQC?',
    a: 'Yes. Bloomhaven Care is registered with and regulated by the Care Quality Commission (CQC). You can view our CQC profile here: <a href="https://www.cqc.org.uk/provider/REPLACE_WITH_CQC_PROVIDER_ID" target="_blank" rel="noopener">Bloomhaven Care CQC profile</a>.'
  },
  {
    q: 'Can care visits be adjusted as needs change over time?',
    a: 'Absolutely. Your care plan evolves with you. We conduct regular reviews and can adjust the frequency, type, and duration of care visits at any time. We believe in proactive, not reactive, care planning.'
  },
  {
    q: 'Do you provide care on weekends and bank holidays?',
    a: 'Yes. We provide care 7 days a week, 365 days a year — including all bank holidays. Care needs don\'t take days off, and neither do we.'
  },
  {
    q: 'What is the difference between home care and a care home?',
    a: 'Home care allows your loved one to remain in their own familiar home, maintaining independence and cherished routines. A care home requires a move to a residential facility. Many families find that high-quality home care — including our live-in care option — offers the best of both worlds: professional support without the upheaval of leaving home.'
  },
  {
    q: 'How does the free home assessment work?',
    a: 'One of our care coordinators will visit you at home, completely free and with no obligation. We\'ll listen carefully to understand your needs, preferences, and goals. The assessment usually takes around an hour and is the foundation for building your personalised care plan.'
  }
];

/* ════════════════════════════════════════════
   [PARTICLES] — Hero canvas system
   ════════════════════════════════════════════ */
function initParticles(canvasId) {
  const canvas = document.getElementById(canvasId);
  if (!canvas) return;
  const ctx = canvas.getContext('2d');
  let mouse = { x: null, y: null };
  let particles = [];
  let raf;

  function resize() {
    canvas.width  = canvas.offsetWidth;
    canvas.height = canvas.offsetHeight;
  }
  resize();
  window.addEventListener('resize', resize, { passive: true });

  canvas.closest('section').addEventListener('mousemove', e => {
    const rect = canvas.getBoundingClientRect();
    mouse.x = e.clientX - rect.left;
    mouse.y = e.clientY - rect.top;
  });

  // Petal shape
  function drawPetal(ctx, x, y, r, angle, alpha) {
    ctx.save();
    ctx.globalAlpha = alpha;
    ctx.translate(x, y);
    ctx.rotate(angle);
    ctx.beginPath();
    ctx.ellipse(0, -r / 2, r / 3, r, 0, 0, Math.PI * 2);
    ctx.fillStyle = `rgba(94,168,50,${alpha})`;
    ctx.fill();
    ctx.restore();
  }

  class Particle {
    constructor() { this.reset(true); }
    reset(initial = false) {
      this.x     = Math.random() * canvas.width;
      this.y     = initial ? Math.random() * canvas.height : canvas.height + 20;
      this.r     = 2 + Math.random() * 5;
      this.vx    = (Math.random() - .5) * .4;
      this.vy    = -(.3 + Math.random() * .6);
      this.angle = Math.random() * Math.PI * 2;
      this.spin  = (Math.random() - .5) * .03;
      this.alpha = .2 + Math.random() * .4;
      this.type  = Math.random() > .4 ? 'petal' : 'dot';
    }
    update() {
      // Mouse repulsion
      if (mouse.x !== null) {
        const dx = this.x - mouse.x, dy = this.y - mouse.y;
        const dist = Math.sqrt(dx * dx + dy * dy);
        if (dist < 120) {
          this.vx += (dx / dist) * .08;
          this.vy += (dy / dist) * .08;
        }
      }
      this.vx *= .98;
      this.vy *= .98;
      this.x += this.vx;
      this.y += this.vy;
      this.angle += this.spin;
      if (this.y < -30) this.reset();
    }
    draw() {
      if (this.type === 'petal') {
        drawPetal(ctx, this.x, this.y, this.r * 2, this.angle, this.alpha * .7);
      } else {
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.r * .6, 0, Math.PI * 2);
        ctx.fillStyle = `rgba(74,158,229,${this.alpha})`;
        ctx.fill();
      }
    }
  }

  // Respect reduced motion
  const motionOK = !window.matchMedia('(prefers-reduced-motion: reduce)').matches;
  if (!motionOK) return;

  const count = Math.min(window.innerWidth < 768 ? 25 : 55, 60);
  for (let i = 0; i < count; i++) particles.push(new Particle());

  function loop() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    particles.forEach(p => { p.update(); p.draw(); });
    raf = requestAnimationFrame(loop);
  }
  loop();
}

/* ════════════════════════════════════════════
   [COUNTERS] — Animated count-up
   ════════════════════════════════════════════ */
function initCounters() {
  const counters = document.querySelectorAll('.stat-num[data-target]');
  if (!counters.length) return;

  const observer = new IntersectionObserver(entries => {
    entries.forEach(entry => {
      if (!entry.isIntersecting) return;
      const el = entry.target;
      const target = parseFloat(el.dataset.target);
      const suffix = el.dataset.suffix || '';
      const prefix = el.dataset.prefix || '';
      const duration = 1800;
      const start = performance.now();

      function tick(now) {
        const t = Math.min((now - start) / duration, 1);
        const eased = 1 - Math.pow(1 - t, 3);
        const value = Math.round(eased * target);
        el.textContent = prefix + value + suffix;
        if (t < 1) requestAnimationFrame(tick);
      }
      requestAnimationFrame(tick);
      observer.unobserve(el);
    });
  }, { threshold: .5 });

  counters.forEach(c => observer.observe(c));
}

/* ════════════════════════════════════════════
   [RENDER] — Services
   ════════════════════════════════════════════ */
function renderServices() {
  const grid = document.getElementById('servicesGrid');
  if (!grid) return;

  services.forEach((s, i) => {
    const image = s.image || SERVICE_IMAGES[i % SERVICE_IMAGES.length];
    const card = document.createElement('div');
    card.className = 'service-card reveal';
    card.setAttribute('role', 'button');
    card.setAttribute('tabindex', '0');
    card.setAttribute('aria-label', `Learn more about ${s.title}`);
    card.setAttribute('data-tag', s.tag);
    card.onclick = () => openModal(i);
    card.onkeydown = e => { if (e.key === 'Enter' || e.key === ' ') { e.preventDefault(); openModal(i); } };

    card.innerHTML = `
      <div class="service-img">
        <img src="${image.src}" alt="${image.alt}" loading="lazy">
        <div class="service-tag">${s.tag}</div>
      </div>
      <div class="service-body">
        <h3>${s.title}</h3>
        <p>${s.summary}</p>
        <span class="service-learn" aria-hidden="true">Discover more →</span>
      </div>`;
    grid.appendChild(card);
  });

  initServiceFilter();
}

/* ════════════════════════════════════════════
   [FILTER] — Service cards
   ════════════════════════════════════════════ */
function initServiceFilter() {
  const filterBtns = document.querySelectorAll('.filter-btn');
  const cards = document.querySelectorAll('.service-card');
  if (!filterBtns.length) return;

  filterBtns.forEach(btn => {
    btn.addEventListener('click', () => {
      filterBtns.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
      const tag = btn.dataset.filter;
      cards.forEach(card => {
        const match = tag === 'All' || card.dataset.tag === tag;
        card.classList.toggle('hidden', !match);
      });
    });
  });
}

/* ════════════════════════════════════════════
   [RENDER] — Team
   ════════════════════════════════════════════ */
function renderTeam() {
  const grid = document.getElementById('teamGrid');
  if (!grid) return;

  team.forEach(m => {
    const card = document.createElement('div');
    card.className = 'team-card reveal';
    card.innerHTML = `
      <div class="team-img" aria-hidden="true">${m.emoji}</div>
      <div class="team-body">
        <div class="team-name">${m.name}</div>
        <div class="team-role">${m.role}</div>
        <p class="team-bio">${m.bio}</p>
        <div class="team-quals">${m.quals.map(q => `<span class="qual-tag">${q}</span>`).join('')}</div>
      </div>`;
    grid.appendChild(card);
  });
}

/* ════════════════════════════════════════════
   [RENDER] — Skills
   ════════════════════════════════════════════ */
function renderSkills() {
  const grid = document.getElementById('skillsGrid');
  if (!grid) return;

  skills.forEach((s, i) => {
    const card = document.createElement('div');
    card.className = 'skill-card reveal';
    card.innerHTML = `
      <div class="skill-num" aria-hidden="true">${String(i + 1).padStart(2, '0')}</div>
      <div class="skill-content">
        <h4>${s.title}</h4>
        <p>${s.desc}</p>
      </div>`;
    grid.appendChild(card);
  });
}

/* ════════════════════════════════════════════
   [FAQ] — Accordion + live search
   ════════════════════════════════════════════ */
function renderFAQ() {
  const list = document.getElementById('faqList');
  if (!list) return;

  faqs.forEach(f => {
    const item = document.createElement('div');
    item.className = 'faq-item reveal';
    item.innerHTML = `
      <div class="faq-q" role="button" tabindex="0" aria-expanded="false">
        <span>${f.q}</span>
        <div class="faq-icon" aria-hidden="true">+</div>
      </div>
      <div class="faq-a" role="region">${f.a}</div>`;

    const q = item.querySelector('.faq-q');
    q.addEventListener('click', () => toggleFAQ(item, q));
    q.addEventListener('keydown', e => {
      if (e.key === 'Enter' || e.key === ' ') { e.preventDefault(); toggleFAQ(item, q); }
    });
    list.appendChild(item);
  });

  initFAQSearch();
}

function toggleFAQ(item, trigger) {
  const isOpen = item.classList.toggle('open');
  trigger.setAttribute('aria-expanded', isOpen);
}

function initFAQSearch() {
  const input = document.getElementById('faqSearch');
  const items = document.querySelectorAll('.faq-item');
  if (!input || !items.length) return;

  input.addEventListener('input', () => {
    const q = input.value.toLowerCase().trim();
    items.forEach(item => {
      const text = item.querySelector('.faq-q span').textContent.toLowerCase();
      item.classList.toggle('filtered-out', q.length > 0 && !text.includes(q));
    });
  });
}

/* ════════════════════════════════════════════
   [MODAL] — Service detail
   ════════════════════════════════════════════ */
function openModal(index) {
  const s = services[index];
  const overlay = document.getElementById('serviceModal');
  if (!overlay) return;

  document.getElementById('modalIcon').textContent = s.icon;
  document.getElementById('modalTitleText').textContent = s.title;
  document.getElementById('modalTagText').textContent = s.tag;
  document.getElementById('modalDesc').textContent = s.description;
  document.getElementById('modalIncludes').innerHTML =
    s.includes.map(item => `<div class="modal-item">${item}</div>`).join('');

  overlay.classList.add('open');
  overlay.querySelector('.modal').focus();
  document.body.style.overflow = 'hidden';
}

function closeModalOutside(e) {
  if (e.target === document.getElementById('serviceModal')) closeModalDirect();
}

function closeModalDirect() {
  const modal = document.getElementById('serviceModal');
  if (modal) modal.classList.remove('open');
  document.body.style.overflow = '';
}

document.addEventListener('keydown', e => {
  if (e.key === 'Escape') closeModalDirect();
});

/* ════════════════════════════════════════════
   [FORM] — Contact form (FormSubmit.co)
   ════════════════════════════════════════════ */
async function submitForm() {
  const first   = document.getElementById('firstName')?.value.trim();
  const last    = document.getElementById('lastName')?.value.trim();
  const email   = document.getElementById('emailField')?.value.trim();
  const phone   = document.getElementById('phoneField')?.value.trim();
  const reason  = document.getElementById('reasonField')?.value;
  const message = document.getElementById('messageField')?.value.trim();
  const consent = document.getElementById('consentCheck')?.checked;
  const referral = document.getElementById('referralField')?.value;

  if (!first || !last || !email || !reason || !message) {
    showFormError('Please fill in all required fields (marked with *).'); return;
  }
  if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email)) {
    showFormError('Please enter a valid email address.'); return;
  }
  if (!consent) {
    showFormError('Please agree to our privacy policy to continue.'); return;
  }

  const btn = document.querySelector('.btn-submit');
  if (btn) { btn.textContent = 'Sending…'; btn.disabled = true; }

  try {
    const data = new FormData();
    data.append('name',         `${first} ${last}`);
    data.append('email',        email);
    data.append('phone',        phone || 'Not provided');
    data.append('enquiry_type', reason);
    data.append('message',      message);
    if (referral) data.append('referral', referral);
    data.append('_subject',     FORM_CONFIG.subject);
    data.append('_captcha',     'false');
    data.append('_template',    'table');
    if (FORM_CONFIG.cc) data.append('_cc', FORM_CONFIG.cc);

    const res = await fetch(`https://formsubmit.co/${CONTACT_EMAIL}`, {
      method: 'POST', body: data, headers: { Accept: 'application/json' }
    });

    if (res.ok) {
      showFormSuccess();
    } else {
      throw new Error('Server error');
    }
  } catch {
    showFormError('Something went wrong. Please try calling us directly on 07436 125564.');
    if (btn) { btn.textContent = 'Send Message →'; btn.disabled = false; }
  }
}

function showFormError(msg) {
  const existing = document.getElementById('formError');
  if (existing) existing.remove();
  const err = document.createElement('p');
  err.id = 'formError';
  err.style.cssText = 'color:#c0392b;font-size:.85rem;margin-bottom:14px;padding:12px 16px;background:#fdf0f0;border-radius:8px;border:1px solid #f5c6cb;';
  err.textContent = '⚠ ' + msg;
  const btn = document.querySelector('.btn-submit');
  if (btn) btn.parentNode.insertBefore(err, btn);
}

function showFormSuccess() {
  const main = document.getElementById('formMain');
  const success = document.getElementById('formSuccess');
  if (main) main.style.display = 'none';
  if (success) success.classList.add('show');
}

function resetForm() {
  ['firstName','lastName','emailField','phoneField','messageField'].forEach(id => {
    const el = document.getElementById(id);
    if (el) el.value = '';
  });
  ['reasonField','referralField'].forEach(id => {
    const el = document.getElementById(id);
    if (el) el.selectedIndex = 0;
  });
  const cb = document.getElementById('consentCheck');
  if (cb) cb.checked = false;
  const err = document.getElementById('formError');
  if (err) err.remove();
  const btn = document.querySelector('.btn-submit');
  if (btn) { btn.textContent = 'Send Message →'; btn.disabled = false; }
  const main = document.getElementById('formMain');
  const success = document.getElementById('formSuccess');
  if (main) main.style.display = 'block';
  if (success) success.classList.remove('show');
}

/* ════════════════════════════════════════════
   [APPLY] — Quick apply form (recruit page)
   ════════════════════════════════════════════ */
async function submitApply() {
  const name    = document.getElementById('applyName')?.value.trim();
  const email   = document.getElementById('applyEmail')?.value.trim();
  const phone   = document.getElementById('applyPhone')?.value.trim();
  const why     = document.getElementById('applyWhy')?.value.trim();

  if (!name || !email || !why) {
    showApplyError('Please fill in your name, email, and why you want to care.'); return;
  }
  if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email)) {
    showApplyError('Please enter a valid email address.'); return;
  }

  const btn = document.getElementById('applyBtn');
  if (btn) { btn.textContent = 'Sending…'; btn.disabled = true; }

  try {
    const data = new FormData();
    data.append('name',    name);
    data.append('email',   email);
    data.append('phone',   phone || 'Not provided');
    data.append('message', why);
    data.append('_subject', 'New Carer Application — Bloomhaven Care');
    data.append('_captcha', 'false');
    data.append('_template', 'table');

    const res = await fetch(`https://formsubmit.co/${CONTACT_EMAIL}`, {
      method: 'POST', body: data, headers: { Accept: 'application/json' }
    });

    if (res.ok) {
      const wrap = document.getElementById('applyWrap');
      if (wrap) wrap.innerHTML = `
        <div style="text-align:center;padding:20px 0;color:var(--cream)">
          <div style="font-size:2.5rem;margin-bottom:12px">💚</div>
          <h3 style="font-family:'Playfair Display',serif;font-size:1.5rem;margin-bottom:8px">Application received!</h3>
          <p style="color:rgba(252,251,247,.75)">Thank you ${name}. We'll be in touch within two working days.</p>
        </div>`;
    } else {
      throw new Error('Error');
    }
  } catch {
    showApplyError('Something went wrong. Please email us directly at info@bloomhavencare.co.uk');
    if (btn) { btn.textContent = 'Send Application →'; btn.disabled = false; }
  }
}

function showApplyError(msg) {
  const existing = document.getElementById('applyError');
  if (existing) existing.remove();
  const err = document.createElement('p');
  err.id = 'applyError';
  err.style.cssText = 'color:#ffd3d3;font-size:.85rem;margin-bottom:14px;padding:10px 16px;background:rgba(255,100,100,.15);border-radius:8px;border:1px solid rgba(255,100,100,.3);';
  err.textContent = '⚠ ' + msg;
  const btn = document.getElementById('applyBtn');
  if (btn) btn.parentNode.insertBefore(err, btn);
}
