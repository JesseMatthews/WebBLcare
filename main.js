/*
  main.js — Bloomhaven Care
  ─────────────────────────────────────────────────────────────
  Contains:
    [CONFIG]   — Your email address for the contact form
    [DATA]     — Services, team members, skills, FAQs
    [RENDER]   — Functions that build HTML from data arrays
    [MODAL]    — Service popup open/close (services.html only)
    [FORM]     — Contact form submit (contact.html only)
  ─────────────────────────────────────────────────────────────
  Edit the DATA section to update all content without
  touching the HTML files.
*/

/* ════════════════════════════════════════════════════════
   [CONFIG] — CONTACT FORM EMAIL
   ════════════════════════════════════════════════════════

   HOW TO SET UP (FREE — no email server needed):
   ────────────────────────────────────────────────
   We use FormSubmit.co — completely free, no sign-up.

   1. Replace the email below with YOUR email address
   2. Upload the site to GitHub Pages
   3. Submit the contact form once — FormSubmit will send
      you a ONE-TIME confirmation email. Click the link.
   4. Done! All future form submissions arrive in your inbox.

   Free forever · No account · Spam filter included
*/

/* ▼▼▼ REPLACE WITH YOUR EMAIL ADDRESS ▼▼▼ */
const CONTACT_EMAIL = 'Info@bloomhavencare.co.uk';
/* ▲▲▲ REPLACE WITH YOUR EMAIL ADDRESS ▲▲▲ */

/* Optional: customise the subject line and CC */
const FORM_CONFIG = {
  subject: 'New Enquiry — Bloomhaven Care Enquiry',
  cc: '',   /* Add a second email here, or leave blank */
};

/* ════════════════════════════════════════════════════════
   [DATA] — SERVICES
   Each object = one card on services.html + a popup modal.
   Edit title, summary, description and includes freely.
   CHANGE: Replace gradient/emoji with real image src when
   you have photos (see README for how).
   ════════════════════════════════════════════════════════ */
const services = [
  {
    title: 'Personal Care',
    icon: '🛁', tag: 'Daily Living', emoji: '🛁',
    gradient: 'linear-gradient(135deg,#a8cde8,#4a90c4,#2e6fa3)',
    summary: 'Dignified, sensitive support with washing, dressing, and personal hygiene.',
    description: 'Our personal care service provides respectful, sensitive support with all aspects of personal hygiene and daily grooming. Our carers are trained to maintain the dignity and privacy of every client throughout.',
    includes: ['Bathing and showering assistance','Hair washing and styling','Dressing and undressing','Oral hygiene and dental care','Skincare and moisturising','Toileting and continence support','Shaving and grooming']
  },
  {
    title: 'Medication Support',
    icon: '💊', tag: 'Health', emoji: '💊',
    gradient: 'linear-gradient(135deg,#b2d4bb,#6ba87a,#4d8a5c)',
    summary: 'Safe, accurate medication prompting and administration by trained carers.',
    description: 'Our trained carers provide reliable support to ensure medications are taken correctly, at the right time, every time — giving clients and families complete peace of mind.',
    includes: ['Medication reminders and prompting','Administration of prescribed medicines','Ordering repeat prescriptions','Liaising with pharmacies and GPs','Medication record keeping','Monitoring for side effects']
  },
  {
    title: 'Companionship',
    icon: '❤️', tag: 'Wellbeing', emoji: '❤️',
    gradient: 'linear-gradient(135deg,#eef5fb,#a8cde8,#6ba87a)',
    summary: 'Meaningful social connection and emotional support to combat loneliness.',
    description: 'Our companionship service goes beyond simply being present — our carers build genuine relationships, sharing interests, conversations, and experiences with the people they support.',
    includes: ['Friendly conversation and social interaction','Accompanying to appointments and outings','Help with hobbies and interests','Reading, puzzles and activities','Support attending community events','Technology help (video calls with family)','Light emotional support']
  },
  {
    title: 'Meal Preparation',
    icon: '🍽️', tag: 'Nutrition', emoji: '🍽️',
    gradient: 'linear-gradient(135deg,#b2d4bb,#4d8a5c,#2e6fa3)',
    summary: 'Nutritious, home-cooked meals tailored to dietary needs and preferences.',
    description: 'Our carers prepare delicious, nutritious meals following individual dietary requirements, cultural preferences, and medical guidelines — ensuring every client is well nourished.',
    includes: ['Breakfast, lunch and dinner preparation','Tailored menus for dietary needs','Grocery shopping assistance','Monitoring food and fluid intake','Special diet support (diabetic, puréed, etc.)','Kitchen tidying and washing up','Batch cooking and meal planning']
  },
  {
    title: 'Housekeeping',
    icon: '🧹', tag: 'Home', emoji: '🧹',
    gradient: 'linear-gradient(135deg,#f0f7f2,#b2d4bb,#6ba87a)',
    summary: 'Light domestic support to keep the home clean, safe, and comfortable.',
    description: 'Our housekeeping service covers essential domestic tasks that keep a home comfortable and safe, allowing clients to focus on enjoying their day.',
    includes: ['Vacuuming and sweeping floors','Dusting and surface cleaning','Bathroom and kitchen cleaning','Laundry and ironing','Changing bed linen','Taking out bins','Light garden tidying']
  },
  {
    title: 'Dementia Care',
    icon: '🧠', tag: 'Specialist', emoji: '🧠',
    gradient: 'linear-gradient(135deg,#1a2e20,#2c4a32,#4a90c4)',
    summary: 'Specialist, compassionate support for people living with dementia.',
    description: 'Our dementia-trained carers use person-centred approaches to support clients at all stages of the condition with dignity, patience, and specialist knowledge.',
    includes: ['Specialist dementia care training','Consistent carer assignment','Routine and structure maintenance','Reminiscence therapy activities','Behavioural and emotional support','Family communication and guidance','Safe home environment support']
  },
  {
    title: 'Live-In Care',
    icon: '🏡', tag: '24/7 Support', emoji: '🏡',
    gradient: 'linear-gradient(135deg,#a8cde8,#6ba87a,#2e6fa3)',
    summary: 'Round-the-clock care from a live-in carer — an alternative to residential care.',
    description: 'Live-in care allows individuals to remain in their own home with round-the-clock support from a dedicated, trained carer — a preferred alternative to a care home.',
    includes: ['24-hour care and support','Sleep-in or waking nights available','All daily living tasks covered','Companionship throughout the day','Specialist care as required','Regular carer rotations for continuity','Full family communication and updates']
  },
  {
    title: 'Mobility Support',
    icon: '🦽', tag: 'Physical Support', emoji: '🦽',
    gradient: 'linear-gradient(135deg,#eef5fb,#a8cde8,#4a90c4)',
    summary: 'Safe assistance with movement, transfers, and getting around the home.',
    description: 'Our carers receive full manual handling training and work in partnership with physiotherapists and OTs to support safe movement in and around the home.',
    includes: ['Safe transfers (bed, chair, toilet)','Walking and mobility aid support','Exercise and rehabilitation support','Falls prevention and awareness','Hoist and equipment use','Physiotherapy exercise encouragement']
  },
  {
    title: 'Night Care',
    icon: '🌙', tag: 'Overnight', emoji: '🌙',
    gradient: 'linear-gradient(135deg,#0d1f16,#1a2e20,#4a90c4)',
    summary: 'Overnight support for clients who need assistance or reassurance at night.',
    description: 'We offer both sleep-in and waking night care. Our night carers provide a reassuring, safe presence and respond immediately to any needs throughout the night.',
    includes: ['Sleep-in carer (available if needed)','Waking night carer (awake throughout)','Toileting and continence support','Medication administration','Repositioning and pressure care','Reassurance for anxiety or dementia','Emergency response readiness']
  }
];

/* ════════════════════════════════════════════════════════
   [DATA] — TEAM MEMBERS
   Shown on team.html. Edit name, role, bio and quals.
   CHANGE: Replace emoji with <img> when you have photos.
   ════════════════════════════════════════════════════════ */
const team = [
  {
    name: 'Name to be Entered Here', role: 'Founder & Director', emoji: '👩',
    bio: 'discription to be added here',
    quals: ['More ','Leadership & Management','More']
  },
    {
    name: 'Name to be added', role: 'Registered Manager', emoji: '👩', 
    bio: 'discription to be added here',
    quals: ['Example: NVQ Level 5 Health & Social Care', 'CQC Compliance Specialist', 'Quality Assurance']
  },
  {
    name: 'Name', role: 'Head of Care Quality', emoji: '👨',
    bio: 'Discription to be added',
    quals: ['Example: NVQ Level 5 Health & Social Care','CQC Compliance Specialist','Quality Assurance']
  },
  {
    name: 'name to be added', role: 'Operations Manager', emoji: '👨‍💼',
    bio: 'David keeps everything running smoothly behind the scenes — from scheduling and logistics to HR and finance — ensuring carers can focus entirely on delivering exceptional care.',
    quals: ['Example: BA Business Management','CIPD HR Practitioner','ILM Level 5']
  },
  {
    name: 'Name to be added', role: 'Lead Care Coordinator', emoji: '👩‍💼',
    bio: 'manages client care plans, conducts assessments, and ensures our carers have everything they need. Clients and families love her warm, reassuring approach.',
    quals: ['NVQ Level 3 Health & Social Care','Mental Health First Aid','Care Coordination']
  },
  {
    name: 'Name to be added', role: 'Training & Development Lead', emoji: '👨‍🏫',
    bio: 'discription to be added here',
    quals: ['Example','NVQ Level 4 Care','Train the Trainer Certified']
  }
];

/* ════════════════════════════════════════════════════════
   [DATA] — SKILLS (shown on recruit.html)
   Top 10 carer skills from NHS / CQC frameworks.
   ════════════════════════════════════════════════════════ */
const skills = [
  { title: 'Compassion & Empathy',        desc: 'The ability to genuinely understand and share the feelings of those you care for. Identified by the NHS as the single most important quality in a care worker.' },
  { title: 'Communication Skills',         desc: 'Clear, kind, and effective communication with clients, families, and healthcare professionals — both verbal and written.' },
  { title: 'Patience & Resilience',        desc: 'Remaining calm, positive, and effective even in challenging situations — essential for delivering consistent, high-quality care.' },
  { title: 'Reliability & Punctuality',    desc: 'Clients depend on carers arriving when expected. Dependability builds trust and is fundamental to safe, quality care delivery.' },
  { title: 'Attention to Detail',          desc: 'Noticing changes in a client\'s condition, following care plans accurately, and maintaining detailed records.' },
  { title: 'Respect for Dignity & Privacy',desc: 'Understanding and upholding the rights of individuals to privacy, autonomy, and dignified treatment at all times — a core CQC standard.' },
  { title: 'Physical Stamina & Fitness',   desc: 'Home care can be physically demanding. Good fitness ensures carers can safely support clients with mobility and personal care tasks.' },
  { title: 'Teamwork & Collaboration',     desc: 'Working effectively alongside colleagues, families, GPs, nurses, and allied health professionals to deliver joined-up care.' },
  { title: 'Problem Solving',              desc: 'The ability to assess situations quickly, make sound judgements, and respond calmly in unexpected circumstances.' },
  { title: 'Commitment to Learning',       desc: 'A willingness to engage with training, reflect on practice, and continuously develop knowledge and skills throughout your career.' }
];

/* ════════════════════════════════════════════════════════
   [DATA] — FAQS (shown on faq.html)
   ════════════════════════════════════════════════════════ */
const faqs = [
  { q: 'How do I arrange home care for myself or a family member?',
    a: 'Simply contact us via phone, email, or the contact form on this website. One of our friendly care coordinators will arrange a free, no-obligation home assessment at a time convenient for you. We\'ll then create a personalised care plan and match you with a suitable carer.' },
  { q: 'How much does home care cost?',
    a: 'Costs vary depending on the type of care, number of visits, and duration. Following your free assessment we\'ll provide a clear, transparent breakdown with no hidden charges. We can also help you explore local authority funding and NHS continuing healthcare.' },
  { q: 'Can I choose my own carer?',
    a: 'We carefully match clients with carers based on compatibility, needs, and preferences. We\'ll always introduce your carer before regular visits begin. If you\'d prefer a different carer, just let us know — your comfort is always our priority.' },
  { q: 'Are your carers trained and vetted?',
    a: 'Absolutely. All Bloomhaven carers undergo enhanced DBS checks, provide multiple references, and complete a comprehensive induction programme before meeting any clients. They also receive ongoing training and regular supervision.' },
  { q: 'What if I need care in an emergency or at short notice?',
    a: 'We do our very best to accommodate urgent care needs. Contact our office team as soon as possible. Existing clients can also reach our 24-hour on-call line outside office hours.' },
  { q: 'Is Bloomhaven Care regulated?',
    a: 'Yes. Bloomhaven Care is registered with and regulated by the Care Quality Commission (CQC). We undergo regular inspections to ensure we maintain the highest standards of care.' },
  { q: 'Can care visits be adjusted as needs change?',
    a: 'Absolutely. Your care plan evolves with you. We conduct regular reviews and can adjust the frequency and type of care visits at any time.' },
  { q: 'Do you provide care on weekends and bank holidays?',
    a: 'Yes. We provide care 7 days a week, 365 days a year, including all bank holidays. Care needs don\'t take days off, and neither do we.' }
];

/* ════════════════════════════════════════════════════════
   [RENDER] — Build HTML cards from data arrays
   Called on page load by each HTML page that needs them.
   ════════════════════════════════════════════════════════ */

/* Service cards → services.html */
function renderServices() {
  const grid = document.getElementById('servicesGrid');
  if (!grid) return;
  services.forEach((s, i) => {
    const card = document.createElement('div');
    card.className = 'service-card';
    card.setAttribute('role', 'button');
    card.setAttribute('tabindex', '0');
    card.setAttribute('aria-label', `Learn more about ${s.title}`);
    card.onclick = () => openModal(i);
    card.onkeydown = e => { if (e.key === 'Enter' || e.key === ' ') openModal(i); };
    card.innerHTML = `
      <div class="service-img">
        <div class="img-ph" style="background:${s.gradient}">${s.emoji}</div>
        <div class="service-img-overlay"></div>
        <div class="service-tag">${s.tag}</div>
      </div>
      <div class="service-body">
        <div class="service-icon">${s.icon}</div>
        <h3>${s.title}</h3>
        <p>${s.summary}</p>
        <span class="service-learn">Learn more →</span>
      </div>`;
    grid.appendChild(card);
  });
}

/* Team cards → team.html */
function renderTeam() {
  const grid = document.getElementById('teamGrid');
  if (!grid) return;
  team.forEach(m => {
    const card = document.createElement('div');
    card.className = 'team-card';
    card.innerHTML = `
      <div class="team-img">
        <div class="team-img-bg"></div>
        <div class="team-avatar">${m.emoji}</div>
      </div>
      <div class="team-body">
        <div class="team-name">${m.name}</div>
        <div class="team-role">${m.role}</div>
        <p class="team-bio">${m.bio}</p>
        <div class="team-quals">${m.quals.map(q => `<span class="qual-tag">${q}</span>`).join('')}</div>
      </div>`;
    grid.appendChild(card);
  });
}

/* Skills cards → recruit.html */
function renderSkills() {
  const grid = document.getElementById('skillsGrid');
  if (!grid) return;
  skills.forEach((s, i) => {
    const card = document.createElement('div');
    card.className = 'skill-card';
    card.innerHTML = `
      <div class="skill-num">${String(i + 1).padStart(2, '0')}</div>
      <div class="skill-content"><h4>${s.title}</h4><p>${s.desc}</p></div>`;
    grid.appendChild(card);
  });
}

/* FAQ accordion → faq.html */
function renderFAQ() {
  const list = document.getElementById('faqList');
  if (!list) return;
  faqs.forEach(f => {
    const item = document.createElement('div');
    item.className = 'faq-item';
    item.innerHTML = `
      <div class="faq-q" role="button" tabindex="0">
        <span>${f.q}</span>
        <div class="faq-icon" aria-hidden="true">+</div>
      </div>
      <div class="faq-a">${f.a}</div>`;
    const q = item.querySelector('.faq-q');
    q.onclick = () => item.classList.toggle('open');
    q.onkeydown = e => { if (e.key === 'Enter' || e.key === ' ') { e.preventDefault(); item.classList.toggle('open'); } };
    list.appendChild(item);
  });
}

/* ════════════════════════════════════════════════════════
   [MODAL] — Service detail popup (services.html only)
   ════════════════════════════════════════════════════════ */
function openModal(index) {
  const s = services[index];
  document.getElementById('modalTitle').textContent = s.title;
  document.getElementById('modalImg').innerHTML =
    `<div class="img-ph" style="background:${s.gradient};font-size:4.5rem;height:100%;">${s.emoji}</div>`;
  document.getElementById('modalDesc').textContent = s.description;
  document.getElementById('modalIncludes').innerHTML =
    s.includes.map(item => `<div class="modal-item">${item}</div>`).join('');
  document.getElementById('serviceModal').classList.add('open');
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

/* Escape key closes modal */
document.addEventListener('keydown', e => { if (e.key === 'Escape') closeModalDirect(); });

/* ════════════════════════════════════════════════════════
   [FORM] — Contact form (contact.html only)
   Uses FormSubmit.co — free, no server needed.
   Set CONTACT_EMAIL at the top of this file.
   ════════════════════════════════════════════════════════ */
async function submitForm() {
  const first   = document.getElementById('firstName')?.value.trim();
  const last    = document.getElementById('lastName')?.value.trim();
  const email   = document.getElementById('emailField')?.value.trim();
  const phone   = document.getElementById('phoneField')?.value.trim();
  const reason  = document.getElementById('reasonField')?.value;
  const message = document.getElementById('messageField')?.value.trim();
  const consent = document.getElementById('consentCheck')?.checked;

  /* Validate */
  if (!first || !last || !email || !reason || !message) {
    showFormError('Please fill in all required fields (marked with *).'); return;
  }
  if (!email.includes('@') || !email.includes('.')) {
    showFormError('Please enter a valid email address.'); return;
  }
  if (!consent) {
    showFormError('Please agree to our privacy policy to continue.'); return;
  }

  /* Demo mode if email not yet configured */
  if (CONTACT_EMAIL === 'YOUR_EMAIL@example.com') {
    console.log('Demo mode — add your email to js/main.js to enable real submissions.');
    showFormSuccess(); return;
  }

  /* Disable button */
  const btn = document.querySelector('.btn-submit');
  btn.textContent = 'Sending…';
  btn.disabled = true;

  try {
    const data = new FormData();
    data.append('name',         `${first} ${last}`);
    data.append('email',        email);
    data.append('phone',        phone || 'Not provided');
    data.append('enquiry_type', reason);
    data.append('message',      message);
    data.append('_subject',     FORM_CONFIG.subject);
    data.append('_captcha',     'false');
    data.append('_template',    'table');
    if (FORM_CONFIG.cc) data.append('_cc', FORM_CONFIG.cc);

    const response = await fetch(`https://formsubmit.co/${CONTACT_EMAIL}`, {
      method: 'POST', body: data, headers: { 'Accept': 'application/json' }
    });

    if (response.ok) {
      showFormSuccess();
    } else {
      showFormError('Something went wrong. Please try calling us directly.');
      btn.textContent = 'Send Message →'; btn.disabled = false;
    }
  } catch (err) {
    showFormError('Unable to send. Please check your connection and try again.');
    btn.textContent = 'Send Message →'; btn.disabled = false;
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
  btn.parentNode.insertBefore(err, btn);
}

function showFormSuccess() {
  document.getElementById('formMain').style.display = 'none';
  document.getElementById('formSuccess').classList.add('show');
}

function resetForm() {
  ['firstName','lastName','emailField','phoneField','messageField'].forEach(id => {
    const el = document.getElementById(id);
    if (el) el.value = '';
  });
  const sel = document.getElementById('reasonField');
  if (sel) sel.selectedIndex = 0;
  const cb = document.getElementById('consentCheck');
  if (cb) cb.checked = false;
  const err = document.getElementById('formError');
  if (err) err.remove();
  const btn = document.querySelector('.btn-submit');
  if (btn) { btn.textContent = 'Send Message →'; btn.disabled = false; }
  document.getElementById('formMain').style.display = 'block';
  document.getElementById('formSuccess').classList.remove('show');
}